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
// [5]  = D42 - right yellow
// [11] = D29 - bottom laurel (top of the two)
// [13] = D37 - left yellow light
// [18] = D41 - right blue
// [27] = D38 - left blue
// [31] = D40 - right red top
// [33] = D11 - bottom blue
// [41] = D39 - top left red

//right laurel
// [1]  = D14 - right laurel {B,C}
// [2]  = D21 - right laurel {C,D}
// [3]  = D28 - right laurel {D,E}
// [9]  = D15 - right laurel {D,C}
// [10] = D22 - right laurel {E,D}
// [14] = D13 - right laurel {A,C}
// [15] = D20 - right laurel {B,D}
// [16] = D27 - right laurel {C,E}
// [20] = D12 - right laurel {H,B}
// [23] = D16 - right laurel {E,C}
// [24] = D23 - right laurel {G,D}
// [28] = D19 - right laurel {A,D}
// [29] = D26 - right laurel {B,E}
// [34] = D18 - right laurel {H,C}
// [37] = D17 - right laurel {G,C}
// [38] = D24 - right laurel{H,D}
// [39] = D25 - right laurel {A,E}


//left laurel
// [0]  = D7  - left laurel {A,B}
// [4]  = D35 - left laurel {E,G}
// [6]  = D6  - left laurel {H,A}
// [7]  = D1  - left laurel {B,A}
// [8]  = D8  - left laurel {C,B}
// [12] = D36 - left laurel {H,G}
// [17] = D34 - left laurel {D,G}
// [19] = D5  - left laurel {G,A}
// [21] = D2  - left laurel {C,A}
// [22] = D9  - left laurel {D,B}
// [25] = D30 - left laurel {H,E}
// [26] = D31 - left laurel {A,G}
// [30] = D33 - left laurel {C,G}
// [32] = D4  - left laurel {E,A}
// [35] = D3  - left laurel {D,A}
// [36] = D10 - left laurel {E,B}
// [40] = D32 - left laurel {B,G}


charlieLed leftLaurel[18] = {
  {E,B}, {H,E}, {D,B}, {A,G}, {C,B}, {B,G}, {A,B}, {C,G},
  {H,A}, {G,A}, {D,G}, {E,A}, {E,G}, {D,A}, {H,G}, {C,A},
  {B,A}
};

charlieLed rightLaurel[18] = {
  {H,B}, {D,E}, {A,C}, {C,E}, {B,C}, {B,E}, {D,C}, {A,E},
  {E,C}, {H,D}, {G,C}, {H,C}, {G,D}, {A,D}, {E,D}, {B,D},
  {C,D}
};

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
    for (byte i=0; i<17; i++){
      myCharlie.ledWrite(rightLaurel[i], 1);
      myCharlie.ledWrite(leftLaurel[i], 1);
      myCharlie.outRow();
    }
    goneTime = millis();
  }
}
