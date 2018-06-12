#include "Chaplex.h"


// [5]  = D42 - right yellow
// [11] = D29 - bottom laurel (top of the two)
// [13] = D37 - left yellow light
// [18] = D41 - right blue
// [27] = D38 - left blue
// [31] = D40 - right red top
// [33] = D11 - bottom blue
// [41] = D39 - top left red


#define A 0 // 12
#define B 1 // 14
#define C 2 // 27
#define D 3 // 19
#define E 4 // 18
#define G 5 // 5
#define H 6 // 17

#define NUM_LAUREL 17
#define PINS 7 //number of these pins
#define FRAME_TIME 50

int current = 0;
int snowCurrent = 16;
int numOfLEDsToShow = 0;
unsigned long time;

// chase animation vars
int tailPosition = 0;
bool direction = true; // true is counting up, false is counting down
bool side = true; // true is right, false is left

byte ctrlpins[] = {12, 14, 27, 19, 18, 5, 17};

Chaplex myCharlie(ctrlpins, PINS); //control instance

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

void setup() {
  randomSeed(analogRead(0));
}

void loop() {
  int randomNumber = random(4);
  if (randomNumber == 1) {
    snowfall();
  } else if (randomNumber == 2) {
    reverseSnowfall();
  } else if (randomNumber == 3) {
    tailChase();
  }else {
    defaultAnimation();
  }
}


void snowfall(){
  while(true){
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
}

void reverseSnowfall(){
  while(true){
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
}

void defaultAnimation(){
  while(true){
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


  Serial.println(time);
}

void tailChase() {
  while(true){
    //handles up/down left right
    if (direction) { // going up
      if(tailPosition < NUM_LAUREL - 1) { // increment the position
        tailPosition++;
      } else { // jump to other side, flip direction
        direction = !direction;
        side = !side;
      }
    } else { // going down
      if (tailPosition > 0) { // decrement the position
        tailPosition--;
      } else { // jump to other side, flip direction
        direction = !direction;
        side = !side;
      }
    }

    int ledIndex = tailPosition;
    if (!side) {
      ledIndex += NUM_LAUREL;
    }

    clearLEDS();
    myCharlie.ledWrite(myLeds[ledIndex], 1);

    // animate frame till next frame
    int burnTime = time + FRAME_TIME;
    while(time < burnTime) {
      time = millis();
      myCharlie.outRow();
    }
  }
}

void clearLEDS() {
  myCharlie.allClear();
  myCharlie.outRow();
}
