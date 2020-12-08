const int LEFT_WING_PIN = A0;
const int LEFT_CENTER_PIN = A1;
const int RIGHT_CENTER_PIN = A2;
const int RIGHT_WING_PIN = A3;

void setup() {
  
  Serial.begin(9600);

  // initialize pins
  pinMode(LEFT_WING_PIN, INPUT);
  pinMode(LEFT_CENTER_PIN, INPUT);
  pinMode(RIGHT_CENTER_PIN, INPUT);
  pinMode(RIGHT_WING_PIN, INPUT);
  
  while (!Serial) {
    // wait until serial connected
    delay(10);
  }

  Serial.print("Sensor Readings");
}

void loop() {
  int lw = analogRead(LEFT_WING_PIN);
  int lc = analogRead(LEFT_CENTER_PIN);
  int rc = analogRead(RIGHT_CENTER_PIN);
  int rw = analogRead(RIGHT_WING_PIN);
  char lineBuff[40];
  sprintf(lineBuff,"LW:%4d  LC:%4d  RC:%4d  RW:%4d\n",lw,lc,rc,rw);
  Serial.print(lineBuff);
}
