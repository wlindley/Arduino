#include <NewPing.h>
#include "RGBLed.h"

const int TRIGGER_PIN = 2;
const int ECHO_PIN = 3;
const int MAX_DISTANCE = 200; //in centimeters

const int RED_PIN = 11;
const int GREEN_PIN = 9;
const int BLUE_PIN = 10;

NewPing sonarSensor(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
RGBLed led(RED_PIN, GREEN_PIN, BLUE_PIN);

void setup() {
  Serial.begin(9600);
  led.on();
}

void loop() {
  delay(50);
  unsigned int microseconds = sonarSensor.ping();
  float distance = microseconds / (float)US_ROUNDTRIP_CM;
  float hue = 360.f * (microseconds / (float)US_ROUNDTRIP_CM) / (float)MAX_DISTANCE;
  Serial.print("Ping: ");
  Serial.print(distance);
  Serial.print("cm, Hue: ");
  Serial.println(hue);
  led.setHSV(hue, 1.f, 1.f);
}
