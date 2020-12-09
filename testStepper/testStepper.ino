#include <Stepper.h>

#define STEPS_REV 64
#define RPM 10
#define LN1 8
#define LN2 9
#define LN3 10
#define LN4 11

Stepper stepper(STEPS_REV, LN1, LN2, LN3, LN4);

void setup() {
  // initialize serial and wait for connection
  Serial.begin(9600);
  while(!Serial){
    delay(10);
  }

  Serial.print("Beginning stepper test. ");
  Serial.print(STEPS_REV);
  Serial.print(" steps per revolution.\n");
  stepper.setSpeed(10);
}

void loop() {
  Serial.print("Enter steps to travel: ");
  int steps = 0;
  // await input from serial
  while(steps == 0){
    steps = Serial.parseInt();
  }
  Serial.print(steps);
  
  // output conversions and travel
  float rot = (steps+0.0)/STEPS_REV;
  Serial.print("\nTraveling ");
  Serial.print(rot);
  Serial.print(" rotations at ");
  Serial.print(RPM);
  Serial.print(" rpm.\n");
  Serial.print("Estimated travel time (seconds): ");
  Serial.print(60*rot/RPM);
  stepper.step(steps);
  Serial.print("\nDone Traveling.\n\n");
}
