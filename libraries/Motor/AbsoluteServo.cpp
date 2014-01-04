#include "AbsoluteServo.h"
#include <Arduino.h>
#include <math.h>

AbsoluteServo::AbsoluteServo()
{
	outputPin = -1;
	prevPos = 0;
}

AbsoluteServo::AbsoluteServo(int iPin)
{
	outputPin = iPin;
	servo.attach(outputPin);
	prevPos = servo.read();
}

void AbsoluteServo::setPercent(float iPercent)
{
	int newPos = constrain(iPercent * 179.f, 0, 179);
	if (prevPos == newPos || !servo.attached())
	{
		return;
	}
	servo.write(newPos);
	prevPos = newPos;
}

float AbsoluteServo::getCurrentPercent()
{
	return servo.read() / 179.f;
}

float AbsoluteServo::getTargetPercent()
{
	return prevPos / 179.f;
}