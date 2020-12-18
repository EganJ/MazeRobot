#include <Stepper.h>

// Sensor pins
#define LW A0 // left wing, currently unused
#define LC A1 // left center
#define RC A2 // right center
#define RW A3 // right wing, currently unused

// Sensor values TODO tune
#define READING_LINE 100
#define READING_OFFLINE 0

// Motor pins
#define RLN1 4
#define RLN2 5
#define RLN3 6
#define RLN4 7

#define LLN1 9
#define LLN2 10
#define LLN3 11
#define LLN4 12

#define STEPS_REV 2038
#define RPM 5

const unsigned msBetweenSteps = (unsigned)(60000/(RPM*STEPS_REV));

int stateL = 0;
int stateR = 0;

int leftPins[4] = {LLN1,LLN2,LLN3,LLN4};
int rightPins[4] = {RLN1,RLN2,RLN3,RLN4};
//Stepper leftSide(STEPS_REV, LLN1, LLN2, LLN3, LLN4);
//Stepper rightSide(STEPS_REV, RLN1, RLN2, RLN3, RLN4);

void setup() {
  for(int i = 0; i < 4; i++){
    pinMode(leftPins[i], OUTPUT);
    pinMode(rightPins[i], OUTPUT);
  }
  pinMode(LW, INPUT);
  pinMode(LC, INPUT);
  pinMode(RC, INPUT);
  pinMode(RW, INPUT);
}

void loop() {
  bool readRight = sensorOnLine(RC);
  bool readLeft = sensorOnLine(LC);
  if(readLeft && !readRight){
    stepLeft(); // left-right mismatch intentional
  }
  else if(readRight && !readLeft){
    stepRight();
  }
  else if(readRight && readLeft){
    stepLeft();
    stepRight();  
  }
  delay(msBetweenSteps);
}

/**
 * True value means sensor on line.
 */
bool sensorOnLine(int pin) {
  return (analogRead(pin) > 500);
}

void stepLeft(){
  for(int i = 0; i <4; i++){
    digitalWrite(leftPins[i], LOW);
  }
  digitalWrite(leftPins[stateL], HIGH);
  stateL = (stateL+1)%4;
}

void stepRight(){
  for(int i = 0; i <4; i++){
    digitalWrite(rightPins[i], LOW);
  }
  digitalWrite(rightPins[3-stateR], HIGH);
  stateR = (stateR+1)%4;
}
