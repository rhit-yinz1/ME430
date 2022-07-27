// Uno
/*
#define REG_DDR_PUSHBUTTON_GREEN DDRD
#define REG_PORT_PUSHBUTTON_GREEN PORTD
#define REG_PIN_PUSHBUTTON_GREEN PIND
#define BIT_PUSHBUTTON_GREEN 2

#define REG_DDR_PUSHBUTTON_YELLOW DDRD
#define REG_PORT_PUSHBUTTON_YELLOW  PORTD
#define REG_PIN_PUSHBUTTON_YELLOW  PIND
#define BIT_PUSHBUTTON_YELLOW  3

#define REG_DDR_PUSHBUTTON_RESET DDRD
#define REG_PORT_PUSHBUTTON_RESET PORTD
#define REG_PIN_PUSHBUTTON_RESET PIND
#define BIT_PUSHBUTTON_RESET 4

#define REG_DDR_LED_GREEN DDRD
#define REG_PORT_LED_GREEN PORTD
#define REG_PIN_LED_GREEN PIND
#define BIT_LED_GREEN 6

#define REG_DDR_LED_YELLOW DDRD
#define REG_PORT_LED_YELLOW PORTD
#define REG_PIN_LED_YELLOW PIND
#define BIT_LED_YELLOW 7
*/

// Mega

#define REG_DDR_PUSHBUTTON_GREEN DDRD
#define REG_PORT_PUSHBUTTON_GREEN PORTD
#define REG_PIN_PUSHBUTTON_GREEN PIND
#define BIT_PUSHBUTTON_GREEN 2

#define REG_DDR_PUSHBUTTON_YELLOW DDRD
#define REG_PORT_PUSHBUTTON_YELLOW  PORTD
#define REG_PIN_PUSHBUTTON_YELLOW  PIND
#define BIT_PUSHBUTTON_YELLOW  3

#define REG_DDR_PUSHBUTTON_RESET DDRD
#define REG_PORT_PUSHBUTTON_RESET PORTD
#define REG_PIN_PUSHBUTTON_RESET PIND
#define BIT_PUSHBUTTON_RESET 0

#define REG_DDR_LED_GREEN DDRH
#define REG_PORT_LED_GREEN PORTH
#define REG_PIN_LED_GREEN PINH
#define BIT_LED_GREEN 3

#define REG_DDR_LED_YELLOW DDRH
#define REG_PORT_LED_YELLOW PORTH
#define REG_PIN_LED_YELLOW PINH
#define BIT_LED_YELLOW 4


#include <LiquidCrystal.h>

volatile uint8_t greenCounter = 0;
volatile uint8_t yellowCounter = 0;
volatile uint8_t mainEventFlag = 0;

#define PUSHBUTTON_FLAG 0x01
#define PUSHBUTTON_FLAG2 0x02
#define PUSHBUTTON_FLAG3 0x04
#define PUSHBUTTON_FLAG4 0x08
#define PUSHBUTTON_FLAG5 0x10
#define PUSHBUTTON_FLAG6 0x20

//LCD constructor
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

void setup()
{
  Serial.begin(115200);
  // setup LCD
  lcd.begin(16, 2); 
  lcd.print("Theodore Yin");
  
  // Pins as input
  REG_DDR_PUSHBUTTON_GREEN &= ~_BV(BIT_PUSHBUTTON_GREEN);
  REG_DDR_PUSHBUTTON_YELLOW &= ~_BV(BIT_PUSHBUTTON_YELLOW);
  REG_DDR_PUSHBUTTON_RESET &= ~_BV(BIT_PUSHBUTTON_RESET);
  
  // Pins as output
  REG_DDR_LED_YELLOW |= _BV(BIT_LED_YELLOW);
  REG_DDR_LED_GREEN |= _BV(BIT_LED_GREEN);

  
  // Enable pull up resistors
  REG_PORT_PUSHBUTTON_GREEN |= _BV(BIT_PUSHBUTTON_GREEN);
  REG_PORT_PUSHBUTTON_YELLOW |= _BV(BIT_PUSHBUTTON_YELLOW);
  REG_PORT_PUSHBUTTON_RESET |= _BV(BIT_PUSHBUTTON_RESET);
  
  
  // Setup the interrupt
  // Uno
  /*
  EIMSK |= _BV(INT0); // turns on INT0 to be interrupt
  EICRA |= _BV(ISC01); // part of falling edge setup
  EICRA &= ~_BV(ISC00); // part of falling edge setup
  
  EIMSK |= _BV(INT1); // turns on INT1 to be interrupt
  EICRA |= _BV(ISC11); // part of falling edge setup
  EICRA &= ~_BV(ISC10); // part of falling edge setup
  */

  //Mega
  
  EIMSK |= _BV(INT2);
  EICRB |= _BV(ISC21);
  EICRB &= ~_BV(ISC20);
  
  EIMSK |= _BV(INT3);
  EICRB |= _BV(ISC31);
  EICRB &= ~_BV(ISC30);

  EIMSK |= _BV(INT0);
  EICRB |= _BV(ISC01);
  EICRB &= ~_BV(ISC00);
  
  sei(); //enable interrupt
  
}

void loop()
{  
  // Reset button without interrupt
  if(mainEventFlag & PUSHBUTTON_FLAG3){
    delay(30);
    if(bit_is_clear(REG_PIN_PUSHBUTTON_RESET, BIT_PUSHBUTTON_RESET)){
        Serial.println(String("RESET"));
        greenCounter = 0;
        yellowCounter = 0;
  	}
    REG_PORT_LED_YELLOW &= ~_BV(BIT_LED_YELLOW);
    REG_PORT_LED_GREEN &= ~_BV(BIT_LED_GREEN);
    mainEventFlag &= ~PUSHBUTTON_FLAG3;
  }
  
  if(mainEventFlag & PUSHBUTTON_FLAG){
    delay(30);
    if(bit_is_clear(REG_PIN_PUSHBUTTON_GREEN, BIT_PUSHBUTTON_GREEN)){
      greenCounter++;
      Serial.println(String("greenCounter = ") + greenCounter);
  	}
    REG_PORT_LED_GREEN |= _BV(BIT_LED_GREEN);
    REG_PORT_LED_YELLOW &= ~_BV(BIT_LED_YELLOW);
    mainEventFlag &= ~PUSHBUTTON_FLAG;
  }
  
  if(mainEventFlag & PUSHBUTTON_FLAG2){
    delay(30);
    if(bit_is_clear(REG_PIN_PUSHBUTTON_YELLOW, BIT_PUSHBUTTON_YELLOW)){
      yellowCounter++;
      Serial.println(String("yellowCounter = ") + yellowCounter);
  	}
    REG_PORT_LED_YELLOW |= _BV(BIT_LED_YELLOW);
    REG_PORT_LED_GREEN &= ~_BV(BIT_LED_GREEN);
    
    mainEventFlag &= ~PUSHBUTTON_FLAG2;
  }
  updateLCD();
}

void updateLCD(){  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting
  lcd.setCursor(0, 1);
  lcd.print("G=");
  lcd.print(greenCounter);
  
  lcd.print("  Y=");
  lcd.print(yellowCounter);
  
  // print the number of seconds since reset:
  lcd.print("  T=");
  lcd.print(millis()/1000);
  

}

//Uno
/*
ISR(INT0_vect){
  mainEventFlag |= PUSHBUTTON_FLAG;
}

ISR(INT1_vect){
  mainEventFlag |= PUSHBUTTON_FLAG2;
}
*/

//Mega

ISR(INT2_vect){
  mainEventFlag |= PUSHBUTTON_FLAG;
}

ISR(INT3_vect){
  mainEventFlag |= PUSHBUTTON_FLAG2;
}

ISR(INT0_vect){
  mainEventFlag |= PUSHBUTTON_FLAG3;
}