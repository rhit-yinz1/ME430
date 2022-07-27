#include <LiquidCrystal.h>

//LCD constructor
LiquidCrystal lcd_1(13, 12, 11, 10, 9, 8);

void setup()
{
  Serial.begin(115200);
  // setup LCD
  lcd_1.begin(16, 2);
  lcd_1.print("Theodore Yin");
}

void loop()
{  
}
