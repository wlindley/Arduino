#include "PinwheelProgram.h"
#include <Arduino.h>

const int MOTOR_PIN = 3;

void PinwheelProgram::init()
{
	button = DigitalSensor(12);
	pinMode(MOTOR_PIN, OUTPUT);
	pinMode(13, OUTPUT);
}

void PinwheelProgram::update(float dt)
{
	digitalWrite(MOTOR_PIN, button.getState() ? HIGH : LOW);
	digitalWrite(13, button.getState() ? HIGH : LOW);
}