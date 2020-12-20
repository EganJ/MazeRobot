// Sensor pins
#define LW A6 // left wing, currently unused
#define LC A5 // left center
#define RC A4 // right center
#define RW A3 // right wing, currently unused

// Motor Config
#define STEPS_REV 2038
#define RPM 5

const unsigned msBetweenSteps = (unsigned)(60000 / (RPM*STEPS_REV));
const int leftPins[4] = {4, 5, 6, 7};
const int rightPins[4] = {12, 11, 10, 9};

/**
   True value means sensor on line.
*/
bool sensorOnLine(int pin);

void stepLeft();

void stepRight();

void stepBackLeft();

void stepBackRight();
