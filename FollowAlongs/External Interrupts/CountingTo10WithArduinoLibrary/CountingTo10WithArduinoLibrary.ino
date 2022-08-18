#define PIN_PUSHBUTTON_PRINT 3
#define PIN_PUSHBUTTON_COUNT 2

volatile uint16_t isrCounter = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("Setup");
  pinMode(PIN_PUSHBUTTON_PRINT, INPUT_PULLUP);
  pinMode(PIN_PUSHBUTTON_COUNT, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(PIN_PUSHBUTTON_PRINT), pushbuttonISR, FALLING);
}

void loop()
{  
  if(digitalRead(PIN_PUSHBUTTON_PRINT){
    Serial.println(String("Counter = ") + isrCounter);
    delay(500);
  }
  delay(10);
}

void pushbuttonISR(){
  isrCounter++;
}

