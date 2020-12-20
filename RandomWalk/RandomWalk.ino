/* Randomly walk through a maze with the following assumptions:
 *  - the maze is set up with only vertical and horizontal lines
 *  - the only time a line terminates is the exit 
 */

#include "RobotFunctions.h"

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
