#include "PinwheelProgram.h"
#include <Arduino.h>

const int MOTOR_PIN = 3;

void PinwheelProgram::init()
{
	pot = PotSensor(A0);
	pinMode(MOTOR_PIN, OUTPUT);
}

void PinwheelProgram::update(float dt)
{
	float potPos = pot.getPercent();
	Serial.println(potPos);
	analogWrite(MOTOR_PIN, potPos * 255);
	//digitalWrite(MOTOR_PIN, button.getState() ? HIGH : LOW);
}