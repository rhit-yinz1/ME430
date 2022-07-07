#include <Servo.h>
#define PIN_PUSHBUTTON 2
#define PIN_LED 3
#define PIN_JOYSTICK_X 0
#define PIN_JOYSTICK_Y 1
#define PIN_SERVO 9


String inputString = "";
bool stringComplete = false;
Servo myServo;

int joystickX;
int lastSentJoystickX;
int joystickY;
int lastSentJoystickY;
const int JOYSTICK_THRESHOLD = 15;

void setup() {
  Serial.begin(9600);
  inputString.reserve(200);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_PUSHBUTTON, INPUT_PULLUP);
  myServo.attach(PIN_SERVO);
}

void loop() {
  if(!digitalRead(2)){
    Serial.println("B");
    delay(500);
  }
  
  //JOYSTICK stuff
  joystickX = analogRead(PIN_JOYSTICK_X);
  if (abs(joystickX - lastSentJoystickX) > JOYSTICK_THRESHOLD) {
    Serial.println(String("X ") + joystickX);
    lastSentJoystickX = joystickX;
  }
  
  joystickY = analogRead(PIN_JOYSTICK_Y);
  if (abs(joystickY - lastSentJoystickY) > JOYSTICK_THRESHOLD) {
    Serial.println(String("Y ") + joystickY);
    lastSentJoystickY = joystickY;
  }
  
  if (stringComplete) {
    inputString.toUpperCase();
    inputString.trim(); //get rid of cr ascii 13
    Serial.println(inputString); // Optional send to MATLAB
    
    if (inputString.equalsIgnoreCase("LED ON")) {
      digitalWrite(PIN_LED, HIGH);
    }
    if (inputString.equalsIgnoreCase("LED OFF")) {
      digitalWrite(PIN_LED, LOW);
    }
    
    if (inputString.startsWith("SERVO ")) { 
      int degrees = inputString.substring(6).toInt();
      myServo.write(degrees);
    }
    
    //CLEAR
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') { //Line Feed ascii 10
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
}