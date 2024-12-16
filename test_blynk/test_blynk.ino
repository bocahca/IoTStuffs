#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL62Et2BjcD"
#define BLYNK_TEMPLATE_NAME "esp8266"
#define BLYNK_AUTH_TOKEN "0kdL6fcm9ho1a3vq3stfzdYzYec7lgvp"


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "NOTE40";
char pass[] = "olleon31";

int switchValue = 0;

BLYNK_WRITE(V0) {
  switchValue = param.asInt(); // Membaca nilai switch (0 atau 1)
  Serial.print("Switch Value: ");
  Serial.println(switchValue);
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop()
{
  Blynk.run();

}

