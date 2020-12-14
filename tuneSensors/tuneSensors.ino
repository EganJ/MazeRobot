const int PIN = A7;
const int threshold = 500;
void setup() {

  Serial.begin(9600);

  // initialize pins
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN, INPUT);

  while (!Serial) {
    // wait until serial connected
    delay(10);
  }

  Serial.print("Sensor");
}

void loop() {
  int val = analogRead(PIN); // supposed to be a digital signal but out of pins
  if (val > 500) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  Serial.print(val);
  Serial.print("\n");
}
