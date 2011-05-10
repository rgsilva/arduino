short ledPin = 11;
short buttonPin = 2;
int ledPower = 0;
 
void setup() {
 
}
 
void loop() {
  int buttonRead = digitalRead(buttonPin);
  while (digitalRead(buttonPin) == HIGH) {
	ledPower++;
	analogWrite(ledPin, ledPower);
	delay(10);
  }
 
  for (; ledPower >= 0; ledPower--) {
	analogWrite(ledPin, ledPower);
	delay(10);
  }
}

