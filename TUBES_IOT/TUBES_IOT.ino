#define BLYNK_TEMPLATE_NAME "esp8266"
#define BLYNK_TEMPLATE_ID "TMPL62Et2BjcD"
#define BLYNK_AUTH_TOKEN "0kdL6fcm9ho1a3vq3stfzdYzYec7lgvp"

#define LED_GREEN D4
#define WATER_SENSOR A0
#define BUZZER D0
#define BUFFER_SIZE 15
#include <BlynkSimpleEsp8266.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>


char ssid[] = "NOTE40";
char pass[] = "olleon31";

int waterLevelBuffer[BUFFER_SIZE];
int bufferIndex = 0;
bool manualControl = false;       // Status tombol manual (0: OFF, 1: ON)
int waterLevel = 0;               // Nilai level air
bool autoLock = false;            // Status auto-lock (keran mati otomatis jika level air > 50)
BlynkTimer timer;                 // Timer untuk monitoring water leve;
    // Timer untuk mengirim data ke Google Sheets


void sendToGoogleSheets(int level) {
  if (WiFi.status() == WL_CONNECTED) { 
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;
    String url = "https://script.google.com/macros/s/AKfycbxqgVGFmdwd1VdCNeXWrLjgQZEmzSfGZEg2ypucq8_3GXukbZKrUigbS1nY9dDfpNxM/exec?level=" + String(level);

    // Tambahkan header untuk permintaan HTTP
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.begin(client, url);                   // Hubungkan ke URL Web App
    int httpResponseCode = http.GET(); // Kirim data melalui GET request

    if (httpResponseCode > 0) {
      Serial.printf("Data sent to Google Sheets. Response: %d\n", httpResponseCode);
    } else {
      Serial.printf("Error sending data: %s\n", http.errorToString(httpResponseCode).c_str());
    }
    http.end(); // Akhiri koneksi HTTP
  } else {
    Serial.println("WiFi not connected.");
  }
}

void sendDataToGoogleSheets() {
   // Kirim nilai terakhir dalam buffer
  int latestWaterLevel = waterLevelBuffer[(bufferIndex + BUFFER_SIZE - 1) % BUFFER_SIZE];
  sendToGoogleSheets(latestWaterLevel);
}

// menyalakan buzzer sebanyak n kali, dengan nilai default n = 5
void soundBuzzer(int n = 5) {
  for (int i = 0; i < n; i++) {
    digitalWrite(BUZZER, HIGH);   // nyalakan buzzer
    delay(500);                   // tunggu 500ms
    digitalWrite(BUZZER, LOW);    // matikan buzzer
    delay(500);                   // tunggu 500ms
  }
}

// fungsi untuk membaca level air dan mengontrol LED
void checkWaterLevel() {
  int batasAir = 70;
  char eventName[] = "level_air";
  int input = analogRead(WATER_SENSOR);

  waterLevel = map(input, 14, 544, 0, 100); // Mapping ke 0-100
  //memastikan level air tidak < 0
  if (waterLevel < 0) {
    waterLevel = 0;
  }
  Serial.printf("Water level: %d\n", waterLevel);

  Blynk.virtualWrite(V1, waterLevel);       // Kirim data level air ke Blynk
  // Simpan ke buffer
  waterLevelBuffer[bufferIndex] = waterLevel;
  bufferIndex = (bufferIndex + 1) % BUFFER_SIZE; // Rotasi indeks

  if (waterLevel > batasAir) {              // jika Level air terlalu tinggi
    autoLock = true;                        // aktifkan auto-lock (keran mati otomatis)
    digitalWrite(LED_GREEN, LOW);
    Blynk.virtualWrite(V0, 0);              // pastikan tombol keran di Blynk mati (0)
    Blynk.logEvent(eventName, "Air penuh, mematikan keran");

    soundBuzzer();                          // nyala buzzer 5 kali
  } else if (waterLevel < 30) {             // level air cukup rendah
    autoLock = false;                       // nonaktifkan auto-lock (keran bisa dinyalakan)
  }
}

// Tombol "Nyala" (Virtual Pin V0)
BLYNK_WRITE(V0) {
  manualControl = param.asInt();            // Baca status tombol (0 = OFF, 1 = ON)

  if (autoLock) {
    digitalWrite(LED_GREEN, LOW);
    Blynk.virtualWrite(V0, 0);
    Serial.println("Auto-lock active! Manual control disabled.");
  } else if (manualControl) {
    digitalWrite(LED_GREEN, HIGH);          // Matikan LED Merah
    Serial.println("Manual: Keran menyala (LED Hijau ON)");
  } else {
    digitalWrite(LED_GREEN, LOW);           // Nyalakan LED Merah
    Serial.println("Manual: Keran mati (LED Hijau OFF)");
  }
}

void setup() {
  // Inisialisasi pin LED, sensor, dan buzzer
  pinMode(LED_GREEN, OUTPUT);
  pinMode(WATER_SENSOR, INPUT);
  pinMode(BUZZER, OUTPUT);

  // Pastikan kedua LED mati di awal
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(BUZZER, LOW); 

  // Mulai koneksi Blynk
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Atur timer untuk memeriksa level air setiap 500 ms
  timer.setInterval(1000L, checkWaterLevel);
  timer.setInterval(15000L, sendDataToGoogleSheets);
  
}

void loop() {
  Blynk.run();  // Jalankan proses Blynk
  timer.run();  // Jalankan timer monitoring

}
