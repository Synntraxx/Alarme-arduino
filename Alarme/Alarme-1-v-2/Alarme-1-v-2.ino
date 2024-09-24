int buttonPin = 2;
int buttonPin2 = 4;

boolean buttonState2 = 0;
boolean buttonState = 0;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(buttonPin2, INPUT);
  Serial.println("Taper bouton 1 pour menu 1");
  Serial.println("Taper bouton 2 pour menu 2");
}

void loop() {

  buttonState2 = digitalRead(buttonPin2);
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    delay(150);
    Serial.println("Menu 1");
  }
  if (buttonState2 == HIGH) {
    delay(150);
    Serial.println("Menu 2");
  }
}
