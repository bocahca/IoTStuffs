#define BLYNK_TEMPLATE_ID "TMPL6KQoX3fou"
#define BLYNK_TEMPLATE_NAME "Project1"
#define BLYNK_AUTH_TOKEN "UEpe5g-c5YGiKEc7wmpS7OkxWtKolzJa"

#include <Blynk.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#define DHTPIN 32
#define DHTTYPE DHT11



// Konfigurasi WiFi
char ssid[] = "NOTE40";
char pass[] = "olleon31";


// Token authentikasi dari Blynk
char auth[] = BLYNK_AUTH_TOKEN;

DHT dht(DHTPIN, DHTTYPE);



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Menghubungkan ke jaringan WiFi
  Serial.print("Menghubungkan ke jaringan WiFi ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Koneksi WiFi berhasil");

  // Menghubungkan ke server Blynk
  Blynk.begin(auth, ssid, pass);
}

void loop() {

  Blynk.run();

  float t = dht.readTemperature();
  float h = dht.readHumidity();



  Blynk.virtualWrite(V0, t);

  Serial.print("suhu : ");

  Serial.println(t);



  delay(200);
}