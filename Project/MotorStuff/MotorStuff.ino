// C++ code
//
void setup()
{
  pinMode(2, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
	
  //Outputs for Dubugging LEDs
  pinMode(3, OUTPUT);
  pinMode(13, OUTPUT);

  
 //Outputs for Transistors, H-Bridge, ect
//   pinMode(11, OUTPUT);
//   pinMode(12, OUTPUT);
//   pinMode(13, OUTPUT);
}

void loop()
{
  //Outputs for debugging LEDs
  digitalWrite(3, !digitalRead(2));
  digitalWrite(13, !digitalRead(12));
  
  //Outputs for Transistors, H-bridges, etc
//   digitalWrite(12, !digitalRead(2));
//   digitalWrite(13, !digitalRead(3));
//   analogWrite(11,analogRead(A0)/4);
//   delay(10);
}