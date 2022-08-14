#include <LiquidCrystal.h>

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

void setup() {
  lcd.begin(16, 2);

  // ADC setup
  // In the datasheet figure out how to Enable the ADC
  // Set the voltage reference
  // 	-- AVCC with external capacitor at AREF pin

  ADCSRA = ADCSRA | 0x40;		// enable the ADC 0b1000 0000
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("ADC 0 = ");
  lcd.print(getAdc0());
  lcd.print("   ");

  lcd.setCursor(0, 1);
  lcd.print("ADC 15 = ");
  lcd.print(getAdc15());
  lcd.print("   ");

  // delay(100);
}

uint16_t getAdc0() {
  return analogRead(0);
  // DONE:
  // Select the pin using the MUX3 to MUX0 bits
  // Start the conversion by setting a bit
  // Do nothing untill the conversion is done
  // return the result
    ADMUX = 0x00;			// ADC0
    ADMUX = ADMUX | 0x40	; // voltage selection
    ADCSRA = ADCSRA | 0x40; // start a conversion
    while( (ADCSRA & 0x10) == 0 ); // wait for conversion to be completed

    return ADC;
}

uint16_t getAdc15() {
  return analogRead(15);
  // DONE:
  // Select the pin using the MUX3 to MUX0 bits
  // Start the conversion by setting a bit
  // Do nothing untill the conversion is done
  // return the result
    ADMUX = 0x0D;			// ADC3
    ADMUX = ADMUX | 0x40	; // voltage selection
    ADCSRA = ADCSRA | 0x40;			// start a conversion
    ADCSRB = ADCSRB | 0x08;
    while( (ADCSRA & 0x10) == 0 );		// wait for conversion to be completed

    return ADC;
}