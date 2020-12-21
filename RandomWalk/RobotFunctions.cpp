#include "RobotFunctions.h"
#include <Arduino.h>

unsigned stateL = 0;
unsigned stateR = 0;

void setupRobot(){
  for (int i = 0; i < 4; i++) {
    pinMode(leftPins[i], OUTPUT);
    pinMode(rightPins[i], OUTPUT);
  }
  pinMode(LW, INPUT);
  pinMode(LC, INPUT);
  pinMode(RC, INPUT);
  pinMode(RW, INPUT);
}

bool sensorOnLine(int pin) {
  return (analogRead(pin) > 500);
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

float cmToSteps(float cm){
  return cm*STEPS_REV/wheelCircCM;
}
