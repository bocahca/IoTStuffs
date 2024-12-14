#define BLYNK_TEMPLATE_ID "TMPL62Et2BjcD"
#define BLYNK_TEMPLATE_NAME "esp8266"
#define BLYNK_AUTH_TOKEN "0kdL6fcm9ho1a3vq3stfzdYzYec7lgvp"

#include <BlynkSimpleEsp8266.h>

char ssid[] = "NOTE40";
char pass[] = "olleon31";
 
#define LED_GREEN D4       
#define WATER_SENSOR A0 
#define BUZZER D0

int manualControl = 0;   // Status tombol manual (0: OFF, 1: ON)
int waterLevel = 0;      // Nilai level air
bool autoLock = false;   // Status auto-lock (keran mati otomatis jika level air > 50)
BlynkTimer timer;

// Fungsi untuk menyalakan buzzer sebanyak 3 kali
void soundBuzzer() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(BUZZER, HIGH);  // Nyalakan buzzer
    delay(500);                   // Tunggu 500ms
    digitalWrite(BUZZER, LOW);   // Matikan buzzer
    delay(500);                   // Tunggu 500ms
  }
}

// Fungsi untuk membaca level air dan mengontrol LED
void checkWaterLevel() {
  waterLevel = analogRead(WATER_SENSOR); 
  waterLevel = map(waterLevel, 15, 502, 0, 100); // Mapping ke 0-100%

  // Kirim data level air ke Blynk
  Blynk.virtualWrite(V1, waterLevel);
  // print juga pada serial monitor
  Serial.print("Water Level: ");
  Serial.println(waterLevel);

  if (waterLevel > 60) { // jika Level air terlalu tinggi
    autoLock = true;     // Aktifkan auto-lock (keran mati otomatis)
    digitalWrite(LED_GREEN, LOW);  
    Blynk.virtualWrite(V0, 0);      // Pastikan tombol "Nyala" di Blynk mati (0)
    Blynk.logEvent("level_air","Air penuh, mematikan keran");

    soundBuzzer(); // Nyala buzzer 3 kali
  } else if (waterLevel < 30) { // Level air cukup rendah
    autoLock = false;    // Nonaktifkan auto-lock (keran bisa dinyalakan)
  }
}

// Tombol "Nyala" (Virtual Pin V0)
BLYNK_WRITE(V0) {
  manualControl = param.asInt(); // Baca status tombol (0 = OFF, 1 = ON)

  if (autoLock) {
    // Jika auto-lock aktif (level air lebih dari 80%), tombol akan dimatikan
    digitalWrite(LED_GREEN, LOW);   
    Blynk.virtualWrite(V0, 0);      // Pastikan tombol di Blynk dalam keadaan OFF

    //Print status auto-lock
    Serial.println("Auto-lock active! Manual control disabled.");
  } else {
    // Jika tombol "Nyala" ditekan
    if (manualControl == 1) {
      digitalWrite(LED_GREEN, HIGH);    // Matikan LED Merah
      Serial.println("Manual: Keran menyala (LED Hijau ON)");
    } else {
      digitalWrite(LED_GREEN, LOW);   // Nyalakan LED Merah
      Serial.println("Manual: Keran mati (LED Hijau OFF)");
    }
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
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Atur timer untuk memeriksa level air setiap 200 ms
  timer.setInterval(500L, checkWaterLevel);
}

void loop() {
  Blynk.run();  // Jalankan proses Blynk
  timer.run();  // Jalankan timer
}
