#include <Stepper.h>

// Sensor pins
#define LW A0 // left wing
#define LC A1 // left center
#define RC A2 // right center
#define RW A3 // right wing

// Sensor values TODO tune
#define READING_LINE 100
#define READING_OFFLINE 0

// Motor pins
#define LLN1 4
#define LLN2 5
#define LLN3 6
#define LLN4 7

#define RLN1 8
#define RLN2 9
#define RLN3 10
#define RLN4 11

#define STEPS_REV 64

Stepper leftSide(STEPS_REV, LLN1, LLN2, LLN3, LLN4);
Stepper rightSide(STEPS_REV, RLN1, RLN2, RLN3, RLN4);

void setup() {
  // put your setup code here, to run once:
  leftSide.setSpeed(30);
  rightSide.setSpeed(30);
}

void loop() {
  // simple bang-bang line following
  if(checkSensor(RC) > checkSensor(LC)){
    // right sensor more on line than left sensor, move left side toward line
    leftSide.step(1);
  }else{
    // right sensor not more on line, move right side toward line
    rightSide.step(1);
  }

}


/**
   Return sensor value on a [0.0, 1.0] range, where 0.0 means a reading off of the line and 1.0 means full on the line.
*/
float checkSensor(int pin) {
  return clamp(0, (float)(analogRead(pin) - READING_OFFLINE) / (READING_LINE - READING_OFFLINE), 1);
}


// General math helper functions

/**
   Binds a value into a [lowerbound, upperbound] range.
   If the value is lower than the lower bound, return lower bound; likewise for upper.
   @param lower - lower bound
   @param val - value to place in range
   @param upper - upper bound
*/
float clamp(float lower, float val, float upper) {
  return max(lower, min( upper, val));
}
