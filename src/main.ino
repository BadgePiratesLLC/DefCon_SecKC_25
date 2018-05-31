#include "Chaplex.h"

#define A 0 // 12
#define B 1 // 14
#define C 2 // 27
#define D 3 // 19
#define E 4 // 18
#define G 5 // 5
#define H 6 // 17

byte ctrlpins[] = {12, 14, 27, 19, 18, 5, 17};

#define PINS 7 //number of these pins

Chaplex myCharlie(ctrlpins, PINS); //control instance

charlieLed myLeds[42]  = {
  {A,B},  {B,C},  {C,D},  {D,E},  {E,G},  {G,H},  {H,A},
  {B,A},  {C,B},  {D,C},  {E,D},  {G,E},  {H,G},  {A,H},
  {A,C},  {B,D},  {C,E},  {D,G},  {E,H},  {G,A},  {H,B},
  {C,A},  {D,B},  {E,C},  {G,D},  {H,E},  {A,G},  {B,H},
  {A,D},  {B,E},  {C,G},  {D,H},  {E,A},  {G,B},  {H,C},
  {D,A},  {E,B},  {G,C},  {H,D},  {A,E},  {B,G},  {C,H}
};

long goneTime;
void setup() {
  randomSeed(analogRead(0));
  goneTime = millis();
}

#define NEWPATTERN 100        //100 ms for new LED pattern
void loop() {
  if (millis()-goneTime >= NEWPATTERN) {
    for (byte i=0; i<100; i++)
      myCharlie.ledWrite(myLeds[i], (byte)random(0,2));
    goneTime = millis();
  }
  myCharlie.outRow();
}
