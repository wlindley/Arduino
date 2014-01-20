#include <NewPing.h>

const int TRIGGER_PIN = 2;
const int ECHO_PIN = 3;
const int MAX_DISTANCE = 200; //in centimeters

NewPing sonarSensor(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(9600);
}

void loop() {
  delay(50);
  unsigned int microseconds = sonarSensor.ping();
  Serial.print("Ping: ");
  Serial.print(microseconds / US_ROUNDTRIP_CM);
  Serial.println("cm");
}
