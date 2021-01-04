/* Main file for the maze solving robot.

Combines the robot functionality with the abstracted version
of the maze to create a fully-fledged maze exploring robot.

Operates on a maze built on a grid with regular distances
between intersections on the grid. Not every intersection on
the grid need be an intersection in the maze, but long 
straightaways could potentially throw off robot odometry.

*/

#include "RobotFunctions.h"
#include "maze.h"

#define mazeWidth 4
#define mazeHeight 4

#define gridSizeCM 10

TremauxTraverser<mazeWidth, mazeHeight> traverser;

direction currentDirection = direction::N;

void setup() {
  Serial.begin(9600);
  setupRobot();
  
  while(!Serial){
    delay(10);
  }
  for(int i =0; i<16;i++){
    // test output characters
    Serial.print(symbolCodes[i]);
  }
}

void loop() {
  traverser.displayMaze();
  traverser.travelInDirection(currentDirection);
  MazeNode nextIntersection = travelToNextIntersection();
  traverser.addCurrentNodeInformation(nextIntersection);
  direction nextDirection = traverser.decideDirection();

  // turn depending on relation between current and next direction
  if (nextDirection == directionToRightOf(currentDirection)) {
    turn90Right();
  } else if (nextDirection == directionToLeftOf(currentDirection)) {
    turn90Left();
  }
  else if (nextDirection == invertDirection(currentDirection)) {
    turn90Left(); // could be right, arbitrary choice
    turn90Left();
  }
  // if none of the above if statements are true we know we are facing in the right direction

  // update information
  currentDirection = nextDirection;
}

/**
   Travels to next intersection and returns the maze node with lead information
   Since the TremauxTraverser expects intersections at regular intervals, track odometry
   to mark the right section of straightaways as intersections.
   With a long enough straightaway, this method might eventually fail.
*/
MazeNode travelToNextIntersection() {
  static const int stepsBetweenIntersection = cmToSteps(gridSizeCM - sensorForwardThrowCM);

  // follow to next intersection
  for (int i = 0; i < stepsBetweenIntersection; i++) {
    followLineSingleStep();
    if (sensorOnLine(LW) || sensorOnLine(RW)) break;
  }

  // investigate intersection
  bool hasStraight = false;
  bool hasRight = false;
  bool hasLeft = false;

  static const int stepsPastIntersection = (int)(cmToSteps(tapeWidthCM + sensorForwardThrowCM)); // add sensor throw so center of rotation is directly on intersection
  for (int i = 0; i < stepsPastIntersection; i++) {
    // check wings for presence of turns
    if ((!hasRight) && sensorOnLine(RW)) {
      hasRight = true;
    }
    if ((!hasLeft) && sensorOnLine(LW)) {
      hasLeft = true;
    }
    // assuming we're straight enough to continue past intersection with no feedback
    stepLeft();
    stepRight();
    delay(msBetweenSteps);
  }
  hasStraight = sensorOnLine(RC) || sensorOnLine(LC);

  // collect all the information we've gathered into a maze node
  MazeNode intersection{};
  intersection.addLeads(invertDirection(currentDirection)); //always have lead the way we came
  if (hasStraight) intersection.addLeads(currentDirection);
  if (hasRight) intersection.addLeads(directionToRightOf(currentDirection));
  if (hasLeft) intersection.addLeads(directionToLeftOf(currentDirection));

  return intersection;
}
