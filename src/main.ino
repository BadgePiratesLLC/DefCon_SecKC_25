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
#define FRAME_TIME 50

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

charlieLed myLeds[42]  = {
  // left laurel
  {E,B}, {H,E}, {D,B}, {A,G}, {C,B}, {B,G}, {A,B}, {C,G},
  {H,A}, {G,A}, {D,G}, {E,A}, {E,G}, {D,A}, {H,G}, {C,A},
  {B,A},
  // right laurel
  {H,B}, {D,E}, {A,C}, {C,E}, {B,C}, {B,E}, {D,C}, {A,E},
  {E,C}, {H,D}, {G,C}, {H,C}, {G,D}, {A,D}, {E,D}, {B,D},
  {C,D}
};
int current = 0;
int snowCurrent = 16;

void setup() {
  randomSeed(analogRead(0));
}

#define NUM_LAUREL 17
int numOfLEDsToShow = 0;
unsigned long time;

void loop() {
  snowfall();
}

void snowfall(){
  unsigned long loopCount = 0;                          // used to determine duty cycle of each LED
  unsigned long timeNow = millis();                     //
  unsigned long displayTime = 10 + random(90);          // milliseconds to spend at each focus LED in descent
  while(millis()- timeNow < (displayTime+current*2)) {  // animation slows toward end
    myCharlie.allClear();
    loopCount++;
    // the "snowflake" gets full duty cycle.  When it gets to the end, hold it at the end until the tail collapses
    myCharlie.ledWrite(myLeds[snowCurrent], 1);
    myCharlie.ledWrite(myLeds[snowCurrent + 17], 1);
    // each member of tail has reduced duty cycle, and never get to the final position
    if(!(loopCount % 3)) if(snowCurrent+1 <=16 && snowCurrent+1 > 0) myCharlie.ledWrite(myLeds[snowCurrent+1], 1);
    if(!(loopCount % 6)) if(snowCurrent+2 <=16 && snowCurrent+2 > 0) myCharlie.ledWrite(myLeds[snowCurrent+2], 1);
    if(!(loopCount % 9)) if(snowCurrent+3 <=16 && snowCurrent+3 > 0) myCharlie.ledWrite(myLeds[snowCurrent+3], 1);
    if(!(loopCount % 12)) if(snowCurrent+4 <=16 && snowCurrent+4 > 0) myCharlie.ledWrite(myLeds[snowCurrent+4], 1);

    if(!(loopCount % 3)) if(snowCurrent+1 <=16 && snowCurrent+1 > 0) myCharlie.ledWrite(myLeds[snowCurrent+17+1], 1);
    if(!(loopCount % 6)) if(snowCurrent+2 <=16 && snowCurrent+2 > 0) myCharlie.ledWrite(myLeds[snowCurrent+17+2], 1);
    if(!(loopCount % 9)) if(snowCurrent+3 <=16 && snowCurrent+3 > 0) myCharlie.ledWrite(myLeds[snowCurrent+17+3], 1);
    if(!(loopCount % 12)) if(snowCurrent+4 <=16 && snowCurrent+4 > 0) myCharlie.ledWrite(myLeds[snowCurrent+17+4], 1);
    myCharlie.outRow();
  }

  snowCurrent--;
  if(snowCurrent<=0) {                          // start over
    // now fade out the snowflake in that final position #19
    for(int dutyCycle = 3; dutyCycle <= 15; dutyCycle += 3) {
      loopCount = 0;
      timeNow = millis();
      while(millis() - timeNow < (displayTime+snowCurrent*2)) { // fade out as slow as animation has achieved by now
        myCharlie.allClear();
        loopCount++;
        if(!(loopCount % dutyCycle)) myCharlie.ledWrite(myLeds[0], 1);
        if(!(loopCount % dutyCycle)) myCharlie.ledWrite(myLeds[17], 1);
        else myCharlie.allClear();
        myCharlie.outRow();
      }
    }
    snowCurrent = 16;
    myCharlie.allClear();
    myCharlie.outRow();            // and then rinse, repeat...after a short pause
  }
}

void reverseSnowfall(){
  unsigned long loopCount = 0;                          // used to determine duty cycle of each LED
  unsigned long timeNow = millis();                     //
  unsigned long displayTime = 10 + random(90);          // milliseconds to spend at each focus LED in descent
  while(millis()- timeNow < (displayTime+current*2)) {  // animation slows toward end
    myCharlie.allClear();
    loopCount++;
    // the "snowflake" gets full duty cycle.  When it gets to the end, hold it at the end until the tail collapses
    myCharlie.ledWrite(myLeds[current], 1);
    myCharlie.ledWrite(myLeds[current + 17], 1);
    // each member of tail has reduced duty cycle, and never get to the final position
    if(!(loopCount % 3)) if(current-1 >=0 && current-1 < 16) myCharlie.ledWrite(myLeds[current-1], 1);
    if(!(loopCount % 6)) if(current-2 >=0 && current-2 < 16) myCharlie.ledWrite(myLeds[current-2], 1);
    if(!(loopCount % 9)) if(current-3 >=0 && current-3 < 16) myCharlie.ledWrite(myLeds[current-3], 1);
    if(!(loopCount % 12)) if(current-4 >=0 && current-4 < 16) myCharlie.ledWrite(myLeds[current-4], 1);

    if(!(loopCount % 3)) if(current-1 >=0 && current-1 < 16) myCharlie.ledWrite(myLeds[current+17-1], 1);
    if(!(loopCount % 6)) if(current-2 >=0 && current-2 < 16) myCharlie.ledWrite(myLeds[current+17-2], 1);
    if(!(loopCount % 9)) if(current-3 >=0 && current-3 < 16) myCharlie.ledWrite(myLeds[current+17-3], 1);
    if(!(loopCount % 12)) if(current-4 >=0 && current-4 < 16) myCharlie.ledWrite(myLeds[current+17-4], 1);
    myCharlie.outRow();
  }

  current++;
  if(current>=16) {                          // start over
    // now fade out the snowflake in that final position #19
    for(int dutyCycle = 3; dutyCycle <= 15; dutyCycle += 3) {
      loopCount = 0;
      timeNow = millis();
      while(millis() - timeNow < (displayTime+current*2)) { // fade out as slow as animation has achieved by now
        myCharlie.allClear();
        loopCount++;
        if(!(loopCount % dutyCycle)) myCharlie.ledWrite(myLeds[16], 1);
        if(!(loopCount % dutyCycle)) myCharlie.ledWrite(myLeds[33], 1);
        else myCharlie.allClear();
        myCharlie.outRow();
      }
    }
    current = 0;
    myCharlie.allClear();
    myCharlie.outRow();            // and then rinse, repeat...after a short pause
  }
}

void defaultAnimation(){
  myCharlie.allClear();
  myCharlie.outRow();

  Serial.print(numOfLEDsToShow);
  if (numOfLEDsToShow < NUM_LAUREL) {
    numOfLEDsToShow = numOfLEDsToShow + 1;
  } else {
    numOfLEDsToShow = 0;
  }

  time = millis();
  int burnTime = time + FRAME_TIME;
  Serial.println(time);
  while(time < burnTime) {
    time = millis();
    for (int i = 0; i < numOfLEDsToShow; i++) {
      myCharlie.ledWrite(myLeds[i], 1);
      myCharlie.ledWrite(myLeds[i+17], 1);
    }

    myCharlie.outRow();
  }
  Serial.println(time);
}
