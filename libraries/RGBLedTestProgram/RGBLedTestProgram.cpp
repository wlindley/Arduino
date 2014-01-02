#include "RGBLedTestProgram.h"
#include <Arduino.h>

void RGBLedTestProgram::init()
{
	Program::init();
	
	outLed = RGBLed(11, 9, 10);
	hue = 0.f;
	value = 0.f;
	valueDelta = .025f;
	
	hueTimer.setDelay(.01f);
	valueTimer.setDelay(.1f);
}

void RGBLedTestProgram::update(float dt)
{
	Program::update(dt);
	updateHue(dt);
	//updateValue(dt);
	outLed.setHSV(hue, 1.f, 1.f);
}

void RGBLedTestProgram::updateHue(float dt)
{
	hueTimer.update(dt);
	
	if (hueTimer.isTriggered())
	{
		hue += 1.f;
		if (hue > 360.f)
		{
			hue -= 360.f;
		}
		hueTimer.reset();
	}
}

void RGBLedTestProgram::updateValue(float dt)
{
	valueTimer.update(dt);
	
	if (valueTimer.isTriggered())
	{
		value += valueDelta;
		if (value >= 1.f || value <= 0.f)
		{
			valueDelta *= -1.f;
		}
		valueTimer.reset();
	}
}