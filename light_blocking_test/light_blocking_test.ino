// Pin yang terhubung ke sensor KY-010
const int sensorPin = 2;

void setup() {
  // Serial untuk debugging
  Serial.begin(115200);

  // Set pin sensor sebagai input
  pinMode(sensorPin, INPUT);
}

void loop() {
  // Baca nilai sensor
  int valDig = digitalRead(sensorPin);
  Serial.print("Nilai Sensor(D): ");
  Serial.println(valDig);
  // Tunda 3 detik
  delay(1000);
}
