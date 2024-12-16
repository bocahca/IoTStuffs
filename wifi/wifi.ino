#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid = "All Hail Megatron";
const char* password = "helloworld";

void setup() {
  Serial.begin(115200);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // Your code here
}
