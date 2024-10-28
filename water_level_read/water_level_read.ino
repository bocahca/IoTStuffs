// Sensor pins
#define sensorPower D7
#define sensorPin A0

// Value for storing water level
int val = 0;
int minimum = 1000;
int maximum = 0;

void setup() {
	pinMode(sensorPower, OUTPUT);   // Set D7 as an OUTPUT
	digitalWrite(sensorPower, LOW); // Set to LOW so no power flows through the sensor
	
	Serial.begin(9600);
}

void loop() {
	//get the reading from the function below and print it
	int level = readSensor();

  minimum = level < minimum ? level : minimum;
  maximum = level > maximum ? level : maximum;

  Serial.printf("(min:%d, max:%d) Water level: %d\n", minimum, maximum, level);
	
	delay(250);
}

int readSensor() {
	int outputValue;
  
  digitalWrite(sensorPower, HIGH);	// Turn the sensor ON
  delay(10);
  val = analogRead(sensorPin);
  outputValue = map(val, 15, 502, 0, 100);
  digitalWrite(sensorPower, LOW);	  // Turn the sensor OFF

	return outputValue;
}