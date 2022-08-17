
#include <SevenSegmentDisplay.h>
#include <Wire.h>

#include "Adafruit_TCS34725.h"

SevenSegmentDisplay sc(25, 23, 26, 24, 22, 27, 29, 28, false);

#define rp 7
#define bp 6
#define gp 5
#define button 4
#define Lbutton 2
#define LLED 3
#define Rbutton 12
#define RLED 13
#define speed 11
#define Lspin 10
#define Rspin 9

#define commonAnode false
byte gammatable[256];
// Create an instance of the TCS34725 Sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

int rn = 0;
int gn = 0;
int bn = 0;
uint16_t score = 0;

void setup() {
  Serial.begin(9600);  // communication
  // RGB Debugging LED Stuff --------
  pinMode(rp, OUTPUT);
  pinMode(bp, OUTPUT);
  pinMode(gp, OUTPUT);
  sc.displayCharacter('0');
  Serial.println("Setting up sensor");
  cksensor();
  gammaSetup();
  Serial.println("Done Sensor");

  // Outputs for Motor Dubugging LEDs
  pinMode(LLED, OUTPUT);
  pinMode(RLED, OUTPUT);

  // Outputs for Motor controll unit
//   attachInterrupt(digitalPinToInterrupt(Lbutton), LbuttonISR, FALLING);

  pinMode(Lbutton, INPUT_PULLUP);
  pinMode(Rbutton, INPUT_PULLUP);
  pinMode(speed, OUTPUT);  // PWM
  pinMode(Lspin, OUTPUT);
  pinMode(Rspin, OUTPUT);

  Serial.println("Done setup");
}

void loop() {
  // turn off debugging leds
  digitalWrite(Lspin, LOW);
  digitalWrite(Rspin, LOW);
  digitalWrite(LLED, LOW);
  digitalWrite(RLED, LOW);
  analogWrite(speed, 200);

  if (!digitalRead(Lbutton)) {
    digitalWrite(Rspin, LOW);
    digitalWrite(Lspin, HIGH);
    digitalWrite(LLED, HIGH);
  }
  if (!digitalRead(Rbutton)) {
    digitalWrite(Lspin, LOW);
    digitalWrite(Rspin, HIGH);
    digitalWrite(RLED, HIGH);
  }

  disColor();

}

void gammaSetup() {
  // thanks PhilB for this gamma table!
  // it helps convert RGB colors to what humans see
  for (int i = 0; i < 256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;

    if (commonAnode) {
      gammatable[i] = 255 - x;
    } else {
      gammatable[i] = x;
    }
    // Serial.println(gammatable[i]);
  }
}

void disColor() {
  // R:	193-180	G:	39	B:	47 red
  // R:	39	G:	128	B:	75 green
  float red, green, blue;

  tcs.setInterrupt(false);  // turn on LED

  delay(60);  // takes 50ms to read

  tcs.getRGB(&red, &green, &blue);

  tcs.setInterrupt(true);  // turn off LED

  Serial.print("R:\t");
  Serial.print(int(red));
  Serial.print("\tG:\t");
  Serial.print(int(green));
  Serial.print("\tB:\t");
  Serial.print(int(blue));

  //  Serial.print("\t");
  //  Serial.print((int)red, HEX); Serial.print((int)green, HEX); Serial.print((int)blue, HEX);
  Serial.print("\n");
  rgb(int(red), int(green), int(blue));
  calcPoint(int(red), int(green), int(blue));
  // wait for 1s to read again
  delay(500);
}

void cksensor() {
  // color sensor intial check
  Serial.println("Color View Test");  // Title info

  if (tcs.begin()) {                                                 // if the sensor starts correctly
    Serial.println("Found sensor");                                  // print the happy message
  } else {                                                           // if the sensor starts incorrectly
    Serial.println("No TCS34725 found ... check your connections");  // print the not so happy message
    while (1);  // halt!
  }
}

void rgb(int r, int g, int b) {
  analogWrite(rp, r);
  analogWrite(gp, g);
  analogWrite(bp, b);
}

void calcPoint(int r, int g, int b){
  if(160<r){
    if(score <= 0){
    }else{
        score--;
    }
    updateScore();
  }
  if(110<g){
    score++;
    updateScore();
  }
}

void updateScore(){
  Serial.print("Score is ");
  Serial.println(score);
  char s = score + '0';
  sc.displayCharacter(s);
}
