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

// [0]  = D7 - left laurel
// [1]  = D14 - right laurel
// [2]  = D21 - right laurel
// [3]  = D28 - right laurel
// [4]  = D35 - left laurel
// [5]  = D42 - right yellow
// [6]  = D6  - left laurel
// [7]  = D1 - left laurel
// [8]  = D8 - left laurel
// [9]  = D15 - right laurel
// [10] = D22 - right laurel
// [11] = D29 - bottom laurel (top of the two)
// [12] = D36 - right laurel
// [13] = D37 - left yellow light
// [14] = D13 - right laurel
// [15] = D20 - right laurel
// [16] = D27 - right laurel
// [17] = D34 - left laurel
// [18] = D41 - right blue
// [19] = D5  - left laurel
// [20] = D12 - right laurel
// [21] = D2 - left laurel
// [22] = D9 - left laurel
// [23] = D16 - right laurel
// [24] = D23 - right laurel
// [25] = D30 - left laurel
// [26] = D31 - left laurel
// [27] = D38 - left blue
// [28] = D19 - right laurel
// [29] = D26 - right laurel
// [30] = D33 - left laurel
// [31] = D40 - right red top
// [32] = D4 - left laurel
// [33] = D11 - bottom blue
// [34] = D18 - right laurel
// [35] = D3 - left laurel
// [36] = D10 - left laurel
// [37] = D17 - right laurel
// [38] = D24 - right laurel
// [39] = D25 - right laurel
// [40] = D32 - left laurel
// [41] = D39 - top left red

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
