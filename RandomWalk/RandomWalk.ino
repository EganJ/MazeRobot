/* Randomly walk through a maze with the following assumptions:
    - all intersections are right angles
    - the only time a line terminates is the exit
*/

#include "RobotFunctions.h"

enum Direction {
  left, right, straight
};

void setup() {
  setupRobot();
}

void loop() {
  followLine();
  Direction turn = investigateIntersection();
  switch (turn) {
    case left:
      turnInPlace(left);
      break;
    case right:
      turnInPlace(right);
      break;
    case straight:
      // already on next line,
      break;
  }

}

/**
   Traverse the space between intersections
*/
void followLine() {
  bool atIntersection = false;
  do {
    //
    atIntersection = sensorOnLine(RW) || sensorOnLine(LW);

    followLineSingleStep();

    delay(msBetweenSteps);
  } while (!atIntersection);
}

void followLineSingleStep() {
  if (sensorOnLine(RC)) {
    stepLeft(); // left-right mismatch intentional
  }
  if (sensorOnLine(LC)) {
    stepRight();
  }
}

Direction investigateIntersection() {
  // going to assume that, due to the distance between stops, the robot is fairly straight.
  // in that case, if we go forward past the intersection and find ourselves on a line, there is a straight option.
  // if, during that time, either of the wing sensors trigger, we know that there is a turn in that direction.
  // Since this is a random walk, once we know which turns are available we can pick a random one.

  // turn option trackers
  bool isStraight = false;
  bool isRight = false;
  bool isLeft = false;

  static const int stepsPerIntersection = (int)(cmToSteps(tapeWidthCM + sensorForwardThrowCM)); // add sensor throw so center of rotation is directly on intersection
  for (int i = 0; i < stepsPerIntersection; i++) {
    // check wings for presence of turns
    if ((!isRight) && sensorOnLine(RW)) {
      isRight = true;
    }
    if ((!isLeft) && sensorOnLine(LW)) {
      isLeft = true;
    }
    stepLeft();
    stepRight();
    delay(msBetweenSteps);
  }

  isStraight = sensorOnLine(RC) || sensorOnLine(LC);

  // randomly select
  Direction options[3];
  int nOpts = 0;
  if (isStraight) {
    options[nOpts] = straight;
    nOpts++;
  }
  if (isRight) {
    options[nOpts] = right;
    nOpts++;
  }
  if (isLeft) {
    options[nOpts] = left;
    nOpts++;
  }
  return options[random(0, nOpts)];
}

// Turn 90 degrees in place
void turnInPlace(Direction dir) {
  // moving left forward and right back, each have to travel 1/4th of the arc of the
  // circle with diameter of the track width. For different directions change +/- steps
  static const int stepsPerWheel = (int)(cmToSteps(PI * trackWidthCM / 4));
  for (int i = 0; i < stepsPerWheel; i++) {
    if (dir == left) {
      stepRight();
      stepBackLeft();
    } else {
      stepLeft();
      stepBackRight();
    }
    delay(msBetweenSteps);
  }
}
