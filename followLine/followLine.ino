#include <Stepper.h>

// Sensor pins
#define LW A6 // left wing, currently unused
#define LC A5 // left center
#define RC A4 // right center
#define RW A3 // right wing, currently unused

// Sensor values TODO tune
#define READING_LINE 100
#define READING_OFFLINE 0

// Motor pins
#define RLN1 12
#define RLN2 11
#define RLN3 10
#define RLN4 9

#define LLN1 4
#define LLN2 5
#define LLN3 6
#define LLN4 7

#define STEPS_REV 2038
#define RPM 5

const unsigned msBetweenSteps = (unsigned)(60000 / (RPM*STEPS_REV));

unsigned stateL = 0;
unsigned stateR = 0;

int leftPins[4] = {LLN1, LLN2, LLN3, LLN4};
int rightPins[4] = {RLN1, RLN2, RLN3, RLN4};
//Stepper leftSide(STEPS_REV, LLN1, LLN2, LLN3, LLN4);
//Stepper rightSide(STEPS_REV, RLN1, RLN2, RLN3, RLN4);

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(leftPins[i], OUTPUT);
    pinMode(rightPins[i], OUTPUT);
  }
  pinMode(LW, INPUT);
  pinMode(LC, INPUT);
  pinMode(RC, INPUT);
  pinMode(RW, INPUT);
}

void loop() {
  bool valLW = sensorOnLine(LW);
  bool valLC = sensorOnLine(LC);
  bool valRC = sensorOnLine(RC);
  bool valRW = sensorOnLine(RW);

  bool turnRight = (valRC || valRW) && !valLW;
  bool turnLeft = (valLC || valLW) && !valRW;
  bool atIntersection = valLW && valRW;

  if (turnRight) {
    stepLeft(); // left-right mismatch intentional
  }
  if (turnLeft) {
    stepRight();
  }
  delay(msBetweenSteps);
}

/**
   True value means sensor on line.
*/
bool sensorOnLine(int pin) {
  return (analogRead(pin) < 500);
}

void stepLeft() {
  stateL = (stateL + 1) % 4;
  for (int i = 0; i < 4; i++) {
    digitalWrite(leftPins[i], LOW);
  }
  digitalWrite(leftPins[3 - stateL], HIGH);
}

void stepRight() {
  stateR = (stateR + 1) % 4;
  for (int i = 0; i < 4; i++) {
    digitalWrite(rightPins[i], LOW);
  }
  digitalWrite(rightPins[stateR], HIGH);
}

void stepBackLeft() {
  stateL = (stateL + 3) % 4; // +3 is congruent to -1 mod 4, don't have to worry about negatives.
  for (int i = 0; i < 4; i++) {
    digitalWrite(leftPins[i], LOW);
  }
  digitalWrite(leftPins[3 - stateL], HIGH);
}

void stepBackRight() {
  stateR = (stateR + 3) % 4; // +3 is congruent to -1 mod 4, don't have to worry about negatives.
  for (int i = 0; i < 4; i++) {
    digitalWrite(rightPins[i], LOW);
  }
  digitalWrite(rightPins[stateR], HIGH);
}
