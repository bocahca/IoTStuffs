int readChar = 0;
int onSecond = 0;
int blinkSecond = 0;
void setup() {

  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  
  if (readChar == '0') { 
    Serial.printf("on %d second(s)\n",onSecond);                  
    digitalWrite(LED_BUILTIN, LOW);   
    delay(1000);
    onSecond++;  
  }else if (readChar == '1')  {
    Serial.printf("Blink %d second(s)\n",blinkSecond);  
    digitalWrite(LED_BUILTIN, LOW);   
    delay(500); 
    digitalWrite(LED_BUILTIN, HIGH);  
    delay(500);
    blinkSecond++; 
  }else {
    digitalWrite(LED_BUILTIN, HIGH); 
  }
  while (Serial.available() > 0) {
    readChar = Serial.read();
    onSecond = 0;
    blinkSecond = 0;   
  }
}
