// Note: arduino pin 13 = ATmega328p (uno) PORTB5
//DDRB
//PORTB
#define REG_DDR_LED DDRD
#define REG_PORT_LED PORTD
#define REG_PIN_LED PIND

#define BIT_LED 6

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
