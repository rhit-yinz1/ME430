#include <Wire.h>  //include the I2C library to communicate with the sensor

#include "Adafruit_TCS34725.h"  //include the sensor library
// rgb debugging led 
#define rp 7
#define bp 6
#define gp 5
#define button 4

byte gammatable[256]

Adafruit_TCS34725 tcs = Adfruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X)

void setup(){
    Serial.begin(115200);

    // Debugging LED setup -----------
    pinMode(rp, OUTPUT);
    pinMode(bp, OUTPUT);
    pinMode(gp, OUTPUT);
    pinMode(4, INPUT_PULLUP);

    //rgb sensor setup -----------
    Serial.println("Color View Test");
    if (tcs.begin()) {                                                 // if the sensor starts correctly
        Serial.println("Found sensor");                                  // print the happy message
    } else {                                                           // if the sensor starts incorrectly
        Serial.println("No TCS34725 found ... check your connections");  // print the not so happy message
        while (1);  // halt!
    }
    // thanks PhilB for this gamma table!
    // it helps convert RGB colors to what humans see
    for (int i = 0; i < 256; i++) {
        float x = i;
        x /= 255;
        x = pow(x, 2.5);
        x *= 255;
        gammatable[i] = x;
        // Serial.println(gammatable[i]);
    }
}

void loop(){
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

    delay(100000000000000000);
}