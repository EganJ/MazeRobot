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

const float wheelCircCM = 18.85;
const float tapeWidthCM = 2.35;
const float trackWidthCM = 10;
const float sensorForwardThrowCM = 3; // how far forward from the center of rotation the sensor are

void setupRobot();

/**
   True value means sensor on line.
*/
bool sensorOnLine(int pin);

void stepLeft();

void stepRight();

void stepBackLeft();

void stepBackRight();

float cmToSteps(float cm);
