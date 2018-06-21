#include "Chaplex.h"
//************************************************************
// this is a simple example that uses the easyMesh library
//
// 1. blinks led once for every node on the mesh
// 2. blink cycle repeats every BLINK_PERIOD
// 3. sends a silly message to every node on the mesh at a random time between 1 and 5 seconds
// 4. prints anything it receives to Serial.print
//
//
//************************************************************
#include <painlessMesh.h>
#include <PinButton.h>

// some gpio pin that is connected to an LED...
// on my rig, this is 5, change to the right number of your LED.
#define   MESH_SSID       "badgepirates-mesh-network"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

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

PinButton myButton(13);

int isMeshOn = 0;
int toggleMesh = 0;

int current = 0;
int snowCurrent = 16;
int numOfLEDsToShow = 0;
unsigned long myTime;

// chase animation vars
int tailPosition = 0;
bool direction = true; // true is counting up, false is counting down
bool side = true; // true is right, false is left

byte ctrlpins[] = {12, 14, 27, 19, 18, 5, 17};

Chaplex myCharlie(ctrlpins, PINS); //control instance

charlieLed indicatorLeds[9] = {
  {G,B},
  {G,H},
  {G,E}, // wifi light
  {A,H},
  {E,H},
  {B,H},
  {D,H},
  {G,B},
  {C,H}
};

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

// Prototypes
void sendMessage();
void receivedCallback(uint32_t from, String & msg);
void newConnectionCallback(uint32_t nodeId);
void changedConnectionCallback();
void nodeTimeAdjustedCallback(int32_t offset);
void delayReceivedCallback(uint32_t from, int32_t delay);

Scheduler     userScheduler; // to control your personal task
painlessMesh  mesh;

bool calc_delay = false;
SimpleList<uint32_t> nodes;

void sendMessage() ; // Prototype
Task taskSendMessage( TASK_SECOND * 1, TASK_FOREVER, &sendMessage ); // start with a one second interval

// Task to blink the number of nodes
// Task blinkNoNodes;
bool onFlag = false;
int numButtonClicks = 0;
int animationState = 0;
int hallEnabled = 0;

volatile int interruptCounter;
int totalInterruptCounter;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
  myCharlie.outRow();
}

void setup() {
  Serial.begin(115200);

  pinMode(13,INPUT_PULLUP);

  randomSeed(analogRead(A0));

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 100, true);
  timerAlarmEnable(timer);
}

void loop() {
  if(isMeshOn == 1) {
    mesh.update();
  }

  userScheduler.execute(); // it will run mesh scheduler as well
  myButton.update();

  if (myButton.isSingleClick()) {
    Serial.println("single");
    // Only triggers on a single, short click (i.e. not
    // on the first click of a double-click, nor on a long click).
    if(animationState >= 2) {
      animationState = 0;
    }
    else {
      animationState++;
    }
  }

  if (myButton.isDoubleClick()) {
    Serial.println("double");
    toggleMesh = 1;
    if (isMeshOn == 0) {
      Serial.println("mesh turned on");
      isMeshOn = 1;
      // initMesh();
    } else {
      isMeshOn = 0;
      Serial.println("mesh turned off");
      // disableMesh();
    }
  }

  getHallReading();
  meshIndicator();
  switch(animationState) {
    case 0:
      snowfall();
      break;
    case 1:
      reverseSnowfall();
      break;
    case 2:
    default:
      defaultAnimation();
      break;
  }

  if (interruptCounter > 0) {
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
    totalInterruptCounter++;
  }
}

void getHallReading() {
  int measurement = 0;

    measurement = hallRead();

    if(measurement >= 50) hallEnabled = 1;
}

void meshIndicator() {
  if(isMeshOn == 1) {
    myCharlie.ledWrite(indicatorLeds[3], 1);
  } else {
    myCharlie.ledWrite(indicatorLeds[3], 0);
  }
}

void initMesh() {
  Serial.println("attempting to enable mesh");
  mesh.setDebugMsgTypes(ERROR | DEBUG | CONNECTION);  // set before init() so that you can see startup messages

  mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  mesh.onNodeDelayReceived(&delayReceivedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
  toggleMesh = 0;
  isMeshOn = 1;
}

void disableMesh() {
  Serial.println("attempting to disable mesh");
  mesh.stop();
  toggleMesh = 0;
  isMeshOn = 0;
}

void sendMessage() {
  String msg = "Hello from node ";
  msg += mesh.getNodeId();
  msg += " myFreeMemory: " + String(ESP.getFreeHeap());
  mesh.sendBroadcast(msg);

  if (calc_delay) {
    SimpleList<uint32_t>::iterator node = nodes.begin();
    while (node != nodes.end()) {
      mesh.startDelayMeas(*node);
      node++;
    }
    calc_delay = false;
  }

  Serial.printf("Sending message: %s\n", msg.c_str());

  taskSendMessage.setInterval( random(TASK_SECOND * 1, TASK_SECOND * 5));  // between 1 and 5 seconds
}

void receivedCallback(uint32_t from, String & msg) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
  // Reset blink task
  onFlag = false;

  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections %s\n", mesh.subConnectionJson().c_str());
  // Reset blink task
  onFlag = false;

  nodes = mesh.getNodeList();

  Serial.printf("Num nodes: %d\n", nodes.size());
  Serial.printf("Connection list:");

  SimpleList<uint32_t>::iterator node = nodes.begin();
  while (node != nodes.end()) {
    Serial.printf(" %u", *node);
    node++;
  }
  Serial.println();
  calc_delay = true;
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void delayReceivedCallback(uint32_t from, int32_t delay) {
  Serial.printf("Delay to node %u is %d us\n", from, delay);
}

void snowfall(){
  unsigned long loopCount = 0;                          // used to determine duty cycle of each LED
  unsigned long timeNow = millis();                     //
  unsigned long displayTime = 10 + random(90);          // milliseconds to spend at each focus LED in descent
  while(millis()- timeNow < (displayTime+current*2)) {  // animation slows toward end
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
  }

  snowCurrent--;
  if(snowCurrent<=0) {                          // start over
    // now fade out the snowflake in that final position #19
    for(int dutyCycle = 3; dutyCycle <= 15; dutyCycle += 3) {
      loopCount = 0;
      timeNow = millis();
      while(millis() - timeNow < (displayTime+snowCurrent*2)) { // fade out as slow as animation has achieved by now
        loopCount++;
        if(!(loopCount % dutyCycle)) {
          myCharlie.ledWrite(myLeds[0], 1);
          myCharlie.ledWrite(myLeds[17], 1);
        } else {
          clearLaurels();
        }
      }
    }
    snowCurrent = 16;
  }
}

void reverseSnowfall() {
  unsigned long loopCount = 0;                          // used to determine duty cycle of each LED
  unsigned long timeNow = millis();                     //
  unsigned long displayTime = 10 + random(90);          // milliseconds to spend at each focus LED in descent
  while(millis()- timeNow < (displayTime+current*2)) {  // animation slows toward end
    clearLaurels();
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
  }

  current++;
  if(current>=16) {                          // start over
    // now fade out the snowflake in that final position #19
    for(int dutyCycle = 3; dutyCycle <= 15; dutyCycle += 3) {
      loopCount = 0;
      timeNow = millis();
      while(millis() - timeNow < (displayTime+current*2)) { // fade out as slow as animation has achieved by now
        loopCount++;
        if(!(loopCount % dutyCycle)) {
          myCharlie.ledWrite(myLeds[16], 1);
          myCharlie.ledWrite(myLeds[33], 1);
        } else {
          clearLaurels();
        }
      }
    }
    current = 0;
    clearLaurels();
  }
}

void defaultAnimation() {
  if (numOfLEDsToShow < NUM_LAUREL) {
    numOfLEDsToShow = numOfLEDsToShow + 1;
  } else {
    numOfLEDsToShow = 0;
  }

  myTime = millis();
  unsigned long burnTime = myTime + FRAME_TIME;
  while(myTime < burnTime) {
    myTime = millis();
    for (int i = 0; i < numOfLEDsToShow; i++) {
      myCharlie.ledWrite(myLeds[i], 1);
      myCharlie.ledWrite(myLeds[i+17], 1);
    }
  }
  clearLaurels();
}

void clearLaurels() {
  for (int i = 0; i < (sizeof(myLeds) / sizeof(myLeds[0])); i++) {
    myCharlie.ledWrite(myLeds[i], 0);
  }
}
