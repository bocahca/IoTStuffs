int FloatSensor=2;

int buttonState = 1; //reads pushbutton status

void setup()
{
  Serial.begin(9600);
  pinMode(FloatSensor, INPUT_PULLUP); //Arduino Internal Resistor 10K
}
void loop()
{
  buttonState = digitalRead(FloatSensor);
  if (buttonState == HIGH)
  {
  Serial.println("WATER LEVEL – LOW"); // kondisi tidak terdeteksi air
  }
  else
  {
  Serial.println("WATER LEVEL – HIGH"); // kondisi terdeteksi air
  }
  delay(1000);
}