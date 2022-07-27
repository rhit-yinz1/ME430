// Timer Planning
// Green Timer to have interrupts every 0.001 seonds via Timer 2
// Plan: Set TIMSK2 to have OCIE2A high enable Timer 2 int
// Prescaler options: (Timer2 can only count to 256)
// 1 --> 0.001 / (1/16000000) = 16000 ticks for 0.001 sec
// 8 --> 0.001 / (8/16000000) = 2000 ticks for 0.001 sec
// 32 --> 0.001 / (32/16000000) = 500
// 64 --> 0.001 / (64/16000000) = 250
// 128 --> 125
// 256 --> 62.5
// 1024 --> 15.625
#define TIMER2_START 0
#define TIMER2_END 250

// Yellow Timer to have interrupts every 0.1 seconds via Timer 1
// PLAN: Set TIMSK1 to have OCIE1A high enable Timer1 int
// Prescaler options: (Timer1 can count to 65536 for UNO)
// 1 --> 0.1 / (1/16000000) = 1600000 ticks for 0.1 sec
// 8 --> 0.1 / (8/16000000) = 200000 ticks for 0.1 sec
// 64 --> 0.1 / (64/16000000) = 25000
// 128 --> 12500
// 256 --> 6250
// 1024 --> 1562.5
#define TIMER1_START 0
#define TIMER1_END 6250

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

// User Timer2 with 0.001 interrupts
bool isGreenTimerRunning = false;
volatile uint16_t greenCounterMilliSec = 0;

// TODO: User Timer 1 with a 0.1 interrupt
bool isYellowTimerRunning = false;
volatile uint16_t yellowCounterTenthSec = 0;

#define DEBOUNCE_TMS 30
volatile uint16_t delayCounter = 0;

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
  
  // Timer2 interrupt setup
  TCCR2A = 0;
  TIMSK2 |= _BV(OCIE2A);
  // PRESCALER: 64 --> 0.001 / (64/16000000) = 250
  TCCR2B |= _BV(CS22);
  TCCR2B &= ~_BV(CS21);
  TCCR2B &= ~_BV(CS20);
  TCNT2 = TIMER2_START;
  OCR2A = TIMER2_END;
  
  // Timer1 interrupt setup
  TCCR1A = 0;
  TIMSK1 |= _BV(OCIE1A);
  // PRESCALER: 256 --> 0.1 / (64/16000000) = 6250
  TCCR1B |= _BV(CS12);
  TCCR1B &= ~_BV(CS11);
  TCCR1B &= ~_BV(CS10);
  TCNT1 = TIMER1_START;
  OCR1A = TIMER1_END;

  sei(); //enable interrupt
  
}

void loop()
{  
  if(mainEventFlag & PUSHBUTTON_FLAG3){
    if(bit_is_clear(REG_PIN_PUSHBUTTON_RESET, BIT_PUSHBUTTON_RESET)){
      Serial.println("RESET");
      REG_PORT_LED_GREEN &= ~_BV(BIT_LED_GREEN);
      REG_PORT_LED_YELLOW &= ~_BV(BIT_LED_YELLOW);
      isGreenTimerRunning = false;
      greenCounterMilliSec = 0;
      isYellowTimerRunning = false;
      yellowCounterTenthSec = 0;
  	}
    mainEventFlag &= ~PUSHBUTTON_FLAG3;
    blockingDelay(DEBOUNCE_TMS);
  }
  if(mainEventFlag & PUSHBUTTON_FLAG){
    if(bit_is_clear(REG_PIN_PUSHBUTTON_GREEN, BIT_PUSHBUTTON_GREEN)){
      // Toggle led and running or not of the timer
      isGreenTimerRunning = !isGreenTimerRunning;
      REG_PORT_LED_GREEN ^= _BV(BIT_LED_GREEN);
  	}
    mainEventFlag &= ~PUSHBUTTON_FLAG;
    blockingDelay(DEBOUNCE_TMS);
  }
  
  if(mainEventFlag & PUSHBUTTON_FLAG2){
    if(bit_is_clear(REG_PIN_PUSHBUTTON_YELLOW, BIT_PUSHBUTTON_YELLOW)){
      isYellowTimerRunning = !isYellowTimerRunning;
      REG_PORT_LED_YELLOW ^= _BV(BIT_LED_YELLOW);
  	}
    mainEventFlag &= ~PUSHBUTTON_FLAG2;
    blockingDelay(DEBOUNCE_TMS);
  }
  updateLCD();
}

void blockingDelay(uint16_t delayMs){
  delayCounter = delayMs;
  while(delayCounter > 0){
  	// Do nothing;
  }
}

void updateLCD(){  
  lcd.setCursor(0, 0);
  uint8_t greenSec = greenCounterMilliSec/1000;
  uint8_t greenTenthSec = (greenCounterMilliSec / 100) % 10;
  lcd.print(greenSec);
  lcd.print(".");
  lcd.print(greenTenthSec);
  lcd.print("   "); // present for reset
  
  if (isYellowTimerRunning){
    lcd.setCursor(0, 1);
    lcd.print("       ");
  }else {
    lcd.setCursor(0, 1);
    uint8_t yellowSec = yellowCounterTenthSec/10;
    uint8_t yellowTenthSec = yellowSec % 10;
    lcd.print(yellowSec);
    lcd.print(".");
    lcd.print(yellowTenthSec);
    lcd.print("   ");
  }
}

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

// Timer Stuff
ISR(TIMER2_COMPA_vect){
  TCNT2 = TIMER2_START;
  if(isGreenTimerRunning){
    greenCounterMilliSec++;
  }
  
  if(delayCounter > 0) {
    delayCounter--;
  }
}
ISR(TIMER1_COMPA_vect){
  TCNT1 = TIMER1_START;
  if(isYellowTimerRunning){
    yellowCounterTenthSec++;
  }
  if(delayCounter > 0) {
    delayCounter--;
  }
}