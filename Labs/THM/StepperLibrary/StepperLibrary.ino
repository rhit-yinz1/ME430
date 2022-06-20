#include <Stepper.h>

const int stepsPerRevolution = 2048;  //32 steps * 64:1 gearhead
// change this to fit the number of steps per revolution
// for your motor
// const int stepsPerRevolution = 60;
// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 7, 9, 8, 10);// order is important

void setup() {
  // set the speed at 5 rpm:
  myStepper.setSpeed(5);
  // initialize the serial port:
}
 
void loop() {
  // step one revolution  in one direction:
  myStepper.step(stepsPerRevolution);
  delay(500);

  // step one revolution in the other direction:
  myStepper.step(-stepsPerRevolution);
  delay(500);
}