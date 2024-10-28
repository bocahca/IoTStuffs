

int i = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  int sensorValue = analogRead(A0);
  // int ledValue = (255.0 / 1024.0) * sensorValue;

  Serial.printf("Sensor Value ke-%d: ", ++i);
  Serial.println(sensorValue);

  digitalWrite(LED_BUILTIN, LOW);   // nyala
  delay(sensorValue);
  digitalWrite(LED_BUILTIN, HIGH);  // mati
  delay(sensorValue);
}