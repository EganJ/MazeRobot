/**
 * Functions for interfacing with robot hardware,
 * along with physical robot information.
 */

#pragma once

// Sensor pins
#define LW A6 // left wing
#define LC A5 // left center
#define RC A4 // right center
#define RW A3 // right wing

// Motor Config
#define STEPS_REV 2038
#define RPM 5

const unsigned msBetweenSteps = (unsigned)(60000 / (RPM*STEPS_REV));
const int leftPins[4] = {4, 5, 6, 7};
const int rightPins[4] = {12, 11, 10, 9};

const float wheelCircCM = 3.1415 * 7.5;
const float tapeWidthCM = 2.5;
const float trackWidthCM = 10;
const float sensorForwardThrowCM = 3; // how far forward from the center of rotation the sensor are

void setupRobot();

bool sensorOnLine(int pin);

/**
 * Move left or right stepper a single step 
 */
void stepLeft();

void stepRight();

void stepBackLeft();

void stepBackRight();

/**
* Based on sensor input, step left or right motors accordingly
*/
void followLineSingleStep();

/**
 * Follow a line until an intersection is hit
 */
void followLine();

void turn90Right();

void turn90Left();

float cmToSteps(float cm);
