// Note: PIN 6 ON ARDUINO = ATmega2560 PH3
//DDRB
//PORTB

// LED_BUILTIN IS PB7
// #define REG_DDR_LED DDRB
// #define REG_PORT_LED PORTB
// #define BIT_LED 

#define REG_DDR_LED DDRH
#define REG_PORT_LED PORTH
#define BIT_LED 3

void setup() {
  //DDRB |= 0b00100000; //set pin 13 as output
  //DDRB |= 0x20; //set pin 13 as output
  //DDRB |= _BV(5);
  //DDRB |= (1 << 5); //set pin 13 as output
  REG_DDR_LED |= _BV(BIT_LED);
}

void loop() {
  REG_PORT_LED |= _BV(BIT_LED);
  delay(1000);
  REG_PORT_LED &= ~_BV(BIT_LED);
  delay(1000);
}
