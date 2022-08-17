#include <SevenSegmentDisplay.h>
int score = 2;
SevenSegmentDisplay sc(25, 23, 26, 24, 22, 27, 29, 28, false);

void setup(){
  char s = score + '0';
  sc.displayCharacter(s);
}

void loop(){

}