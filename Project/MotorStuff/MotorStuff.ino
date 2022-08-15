// C++ code
//
void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);

  // Outputs for Dubugging LEDs
  pinMode(3, OUTPUT);
  pinMode(13, OUTPUT);

  // Outputs for Transistors, H-Bridge, ect
  pinMode(11, OUTPUT);  // PWM
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
}

void loop() {
  // Outputs for debugging LEDs
  digitalWrite(10, LOW);
  digitalWrite(9, LOW);
  digitalWrite(3, LOW);
  digitalWrite(13, LOW);
  analogWrite(11, 200);
  if (!digitalRead(2)) {
    digitalWrite(10, HIGH);
    digitalWrite(9, LOW);
    digitalWrite(3, HIGH);
  } else if (!digitalRead(12)) {
    digitalWrite(10, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(13, HIGH);
  }
  delay(10);
}