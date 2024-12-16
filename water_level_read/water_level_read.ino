// Sensor pins
#define sensorPower D7
#define sensorPin A0
#define buzzerPin D5

// Value for storing water level
int val = 0;
int minimum = 1000;
int maximum = 0;

void setup() {
	pinMode(sensorPower, OUTPUT);
  pinMode(buzzerPin, OUTPUT);   
	digitalWrite(sensorPower, LOW); 	
	Serial.begin(9600);
}

int readSensor() {
	int outputValue;
  digitalWrite(sensorPower, HIGH);
  delay(10);
  val = analogRead(sensorPin);
  // outputValue = map(val, 15, 502, 0, 100);
  digitalWrite(sensorPower, LOW);

	return val;
}

// void setBunyi(bool on = true) {
//   digitalWrite(buzzerPin, on ? HIGH : LOW);
// }

void setBunyi(int n) {
  analogWrite(buzzerPin, n);
}

void loop() {
	int level = readSensor();
  // setBunyi(level > 90);
  int levelBunyi = map(level, 0, 100, 0, 255);
  setBunyi(levelBunyi);
  minimum = level < minimum ? level : minimum;
  maximum = level > maximum ? level : maximum;
  Serial.printf("(min:%d, max:%d) Water level: %d\n", minimum, maximum, level);
	
	delay(500);
}
