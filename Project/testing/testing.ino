/*
 * Adafruit TCS34725 RGB Sensor Demo Code
 *
 * learnelectronics
 * 2 MAR 2018
 *
 * www.youtube.com/c/learnelectronics
 * www.patreon.com/learnelectronics
 * paypal.me/learnelectronics
 *
 * adapted from code by Adafruit
 */

#include <Wire.h>  //include the I2C library to communicate with the sensor

#include "Adafruit_TCS34725.h"  //include the sensor library

#define redpin 4    // pwm output for RED anode use 1K resistor
#define greenpin 5  // pwm output for GREEN anode use 2K resistor
#define bluepin 6   // pwm output for BLUE anode use 1K resistor

#define commonAnode false  // set to false if using a common cathode LED

byte gammatable[256];  // our RGB -> eye-recognized gamma color

// Create an instance of the TCS34725 Sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  Serial.begin(9600);                 // Sart serial comms @ 9600 (you can change this)
  Serial.println("Color View Test");  // Title info

  if (tcs.begin()) {                                                 // if the sensor starts correctly
    Serial.println("Found sensor");                                  // print the happy message
  } else {                                                           // if the sensor starts incorrectly
    Serial.println("No TCS34725 found ... check your connections");  // print the not so happy message
    while (1)
      ;  // halt!
  }

  pinMode(redpin, OUTPUT);    // set redpin for output
  pinMode(greenpin, OUTPUT);  // set greenpin for output
  pinMode(bluepin, OUTPUT);   // set bluepin for output

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
    Serial.println(gammatable[i]);
  }
}

void loop() {
  uint16_t clear, red, green, blue;  // declare variables for the colors

  tcs.setInterrupt(false);  // turn on LED

  delay(60);  // takes 50ms to read

  tcs.getRawData(&red, &green, &blue, &clear);  // read the sensor

  tcs.setInterrupt(true);  // turn off LED

  Serial.print("C:\t");
  Serial.print(clear);  // print color values
  Serial.print("\tR:\t");
  Serial.print(red);
  Serial.print("\tG:\t");
  Serial.print(green);
  Serial.print("\tB:\t");
  Serial.print(blue);

  // Figure out some basic hex code for visualization
  uint32_t sum = clear;
  float r, g, b;
  r = red;
  r /= sum;
  g = green;
  g /= sum;
  b = blue;
  b /= sum;
  r *= 256;
  g *= 256;
  b *= 256;
  Serial.print("\t");
  Serial.print((int)r, HEX);
  Serial.print((int)g, HEX);
  Serial.print((int)b, HEX);
  Serial.println();

  // Serial.print((int)r ); Serial.print(" "); Serial.print((int)g);Serial.print(" ");  Serial.println((int)b );

  analogWrite(redpin, gammatable[(int)r]);    // light red led as per calculation
  analogWrite(greenpin, gammatable[(int)g]);  // light green led as per calculation
  analogWrite(bluepin, gammatable[(int)b]);   // light blue led as per calculation
}