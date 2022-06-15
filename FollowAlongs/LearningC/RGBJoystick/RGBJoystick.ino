
int rPot = 0;
int bPot = 0;

void setup()
{
  Serial.begin(9600); //bits per second
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

}

void loop()
{
  red();
  delay(1000);
  off();
  delay(1000);
}

void red(){
  analogWrite(9, 255);
  analogWrite(10, 0);
  analogWrite(11, 0);
}

void green(){
  analogWrite(9, 0);
  analogWrite(10, 0);
  analogWrite(11, 255);
}

void blue(){
  analogWrite(9, 0);
  analogWrite(10, 255);
  analogWrite(11, 0);
}

void rgb(int r, int g, int b) {
  analogWrite(9, r);
  analogWrite(10, b);
  analogWrite(11, g);
}


void off() {
  analogWrite(9, 0);
  analogWrite(10, 0);
  analogWrite(11, 0);
}