#include <Wire.h>
#include "Adafruit_TCS34725.h"

#define rp 7
#define bp 6
#define gp 5
#define button 4

#define commonAnode false
byte gammatable[256];
// Create an instance of the TCS34725 Sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

int rn = 0;
int gn = 0;
int bn = 0;

void setup() {
  Serial.begin(9600);  // bits per second

  // Debugging LED Stuff --------
  pinMode(rp, OUTPUT);
  pinMode(bp, OUTPUT);
  pinMode(gp, OUTPUT);

  pinMode(12, INPUT_PULLUP);

  Serial.println("Setting up sensor");
  cksensor();
  gammaSetup();
  Serial.println("Done setup");
}

void loop() {
  disColor();
}

void cksensor(){
  // color sensor intial check
  Serial.println("Color View Test");  // Title info

  if (tcs.begin()) {                                                 // if the sensor starts correctly
    Serial.println("Found sensor");                                  // print the happy message
  } else {                                                           // if the sensor starts incorrectly
    Serial.println("No TCS34725 found ... check your connections");  // print the not so happy message
    while (1)
      ;  // halt!
  }
}

void disColor(){
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

  analogWrite(rp, gammatable[(int)red]);
  analogWrite(gp, gammatable[(int)green]);
  analogWrite(bp, gammatable[(int)blue]);
}
void gammaSetup(){
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
void red() {
  analogWrite(rp, 255);
  analogWrite(gp, 0);
  analogWrite(bp, 0);
}

void green() {
  analogWrite(rp, 0);
  analogWrite(gp, 255);
  analogWrite(bp, 0);
}

void blue() {
  analogWrite(rp, 0);
  analogWrite(gp, 0);
  analogWrite(bp, 255);
}

void rgb(int r, int g, int b) {
  analogWrite(rp, r);
  analogWrite(gp, g);
  analogWrite(bp, b);
}

void off() {
  analogWrite(rp, 0);
  analogWrite(gp, 0);
  analogWrite(bp, 0);
}