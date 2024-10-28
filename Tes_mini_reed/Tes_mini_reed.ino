
// Pin yang terhubung ke reed sensor
const int reedPin = 19;

void setup() {
  // Serial untuk debugging
  Serial.begin(115200);

  // Set pin reed sensor sebagai input
  pinMode(reedPin, INPUT);
}

void loop() {
  // Baca status reed sensor
  int status = digitalRead(reedPin);
  // Cetak status ke serial
  Serial.print("Status Reed Sensor: ");
  Serial.println(status);

  // Tunda 1 detik
  delay(1000);
}
