// Uno
/*
#define REG_DDR_PUSHBUTTON_PRINT DDRD
#define REG_PORT_PUSHBUTTON_PRINT PORTD
#define REG_PIN_PUSHBUTTON_PRINT PIND
#define BIT_PUSHBUTTON_PRINT 3

#define REG_DDR_PUSHBUTTON_COUNT DDRD
#define REG_PORT_PUSHBUTTON_COUNT PORTD
#define REG_PIN_PUSHBUTTON_COUNT PIND
#define BIT_PUSHBUTTON_COUNT 2
*/
// Mega

#define REG_DDR_PUSHBUTTON_PRINT DDRE
#define REG_PORT_PUSHBUTTON_PRINT PORTE
#define REG_PIN_PUSHBUTTON_PRINT PINE
#define BIT_PUSHBUTTON_PRINT 5

#define REG_DDR_PUSHBUTTON_COUNT DDRE
#define REG_PORT_PUSHBUTTON_COUNT PORTE
#define REG_PIN_PUSHBUTTON_COUNT PINE
#define BIT_PUSHBUTTON_COUNT 4


volatile uint16_t isrCounter = 0;
volatile uint8_t mainEventFlag = 0;
#define PUSHBUTTON_FLAG 0x01
#define PUSHBUTTON_FLAG2 0x02
#define PUSHBUTTON_FLAG3 0x04
#define PUSHBUTTON_FLAG4 0x08
#define PUSHBUTTON_FLAG5 0x10
#define PUSHBUTTON_FLAG6 0x20
void setup()
{
  Serial.begin(9600);
  Serial.println("Setup");
  // Pins as input
  REG_DDR_PUSHBUTTON_PRINT &= ~_BV(BIT_PUSHBUTTON_PRINT);
  REG_DDR_PUSHBUTTON_COUNT &= ~_BV(BIT_PUSHBUTTON_COUNT);
  
  // Enable pull up resistors
  REG_PORT_PUSHBUTTON_PRINT |= _BV(BIT_PUSHBUTTON_PRINT);
  REG_PORT_PUSHBUTTON_COUNT |= _BV(BIT_PUSHBUTTON_COUNT);
  
  // Setup the interrupt
  
  // Uno
  /*
  EIMSK |= _BV(INT0); // turns on INT0 to be interrupt
  EICRA |= _BV(ISC01); // part of falling edge setup
  EICRA &= ~_BV(ISC00); // part of falling edge setup
  */

  //Mega
  EIMSK |= _BV(INT4); // turns on INT0 to be interrupt
  EICRB |= _BV(ISC41); // part of falling edge setup
  EICRB &= ~_BV(ISC40); // part of falling edge setup
  sei(); // turn on interrupts! Do this last.
}

void loop()
{  
  if(bit_is_clear(REG_PIN_PUSHBUTTON_PRINT, BIT_PUSHBUTTON_PRINT)){
    Serial.println(String("Counter = ") + isrCounter);
    delay(500);
  }
  if(mainEventFlag & PUSHBUTTON_FLAG){
    delay(30);
    if(bit_is_clear(REG_PIN_PUSHBUTTON_COUNT, BIT_PUSHBUTTON_COUNT)){
        isrCounter++;
    }
    //isButtonPressed = False;
    mainEventFlag &= ~PUSHBUTTON_FLAG;
  }

  // Go to sleep
  
}

//Uno
/*
ISR(INT0_vect){
  isrCounter++; 
}
*/
//Mega

ISR(INT4_vect){
    //isButtonPressed = True;
  mainEventFlag |= PUSHBUTTON_FLAG;
  
}

