#include <Stepper.h>
const int STEPS_REV = 2038; // 64 steps per rev at ~63.8 gearbox reduction
const float RPM = 5;

const unsigned msBetweenSteps = (unsigned)(60000/(RPM*STEPS_REV));

int counter = 0;

int rightPins[4] = {5,4,3,2};
int leftPins[4] = {9,10,11,12};


//Stepper left(STEPS_REV,);
//Stepper right(STEPS_REV,9,10,11,12);

void setup() {
  // if speed not set, they will not delay between steps, meaning we can do manually.
//  left.setSpeed(RPM);
//  right.setSpeed(RPM);
    for(int i = 0; i <4; i++){
      pinMode(leftPins[i], OUTPUT);
      pinMode(rightPins[i], OUTPUT);
    }
}

void loop() {
//  left.step(1);
//  right.step(1);

  for(int i = 0; i <4; i++){
    digitalWrite(leftPins[i], LOW);
    digitalWrite(rightPins[i], LOW);
  }
  digitalWrite(leftPins[3-counter], HIGH);
  digitalWrite(rightPins[counter], HIGH);
  counter = (counter+1)%4;
  delay(msBetweenSteps);
}
