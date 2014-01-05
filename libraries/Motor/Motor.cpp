#include "Motor.h"
#include <Arduino.h>

Motor::Motor() : outputPin(-1), isDigital(true) {}

Motor::Motor(int iPin, bool iIsDigital) : outputPin(iPin), isDigital(iIsDigital)
{
	pinMode(iPin, OUTPUT);
}

void Motor::on()
{
	setSpeed(1.f);
}

void Motor::off()
{
	setSpeed(0.f);
}

void Motor::setSpeed(float iPercent)
{
	if (0 > outputPin)
	{
		return;
	}
	if (isDigital)
	{
		digitalWrite(outputPin, iPercent >= .5f ? HIGH : LOW);
	}
	else
	{
		analogWrite(outputPin, iPercent * 255);
	}
}