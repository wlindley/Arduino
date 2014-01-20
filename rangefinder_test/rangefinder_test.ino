#include <NewPing.h>
#include "RGBLed.h"
#include "UltrasonicRangeSensor.h"

const int TRIGGER_PIN = 2;
const int ECHO_PIN = 3;
const int MAX_DISTANCE = 2; //in meters

const int RED_PIN = 11;
const int GREEN_PIN = 9;
const int BLUE_PIN = 10;

UltrasonicRangeSensor sonarSensor(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
RGBLed led(RED_PIN, GREEN_PIN, BLUE_PIN);

void setup() {
  Serial.begin(9600);
  led.on();
}

void loop() {
  /*unsigned int microseconds = sonarSensor.getMicroSeconds();
  float distance = microseconds / (float)US_ROUNDTRIP_CM;
  float hue = 360.f * (microseconds / (float)US_ROUNDTRIP_CM) / (float)MAX_DISTANCE;
  Serial.print("Ping: ");
  Serial.print(distance);
  Serial.print("cm, Hue: ");
  Serial.println(hue);*/
  led.setHSV(360.f * sonarSensor.getPercent(), 1.f, 1.f);
}
