#define BLYNK_TEMPLATE_NAME "esp8266"
#define BLYNK_TEMPLATE_ID "TMPL62Et2BjcD"
#define BLYNK_AUTH_TOKEN "0kdL6fcm9ho1a3vq3stfzdYzYec7lgvp"

#define LED_GREEN D4
#define WATER_SENSOR A0
#define BUZZER D0

#include <BlynkSimpleEsp8266.h>

char ssid[] = "NOTE40";
char pass[] = "olleon31";

bool manualControl = false;       // Status tombol manual (0: OFF, 1: ON)
int waterLevel = 0;               // Nilai level air
bool autoLock = false;            // Status auto-lock (keran mati otomatis jika level air > 50)
BlynkTimer timer;

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

  waterLevel = map(input, 17, 544, 0, 100); // Mapping ke 0-100
  Serial.printf("Water level: %d\n", waterLevel);

  Blynk.virtualWrite(V1, waterLevel);       // Kirim data level air ke Blynk

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
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Atur timer untuk memeriksa level air setiap 500 ms
  timer.setInterval(500L, checkWaterLevel);
}

void loop() {
  Blynk.run();  // Jalankan proses Blynk
  timer.run();  // Jalankan timer
}
