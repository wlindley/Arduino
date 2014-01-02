#include "RGBLedProgram.h"
#include <Arduino.h>

void RGBLedProgram::init()
{
	Program::init();
	outLed = new RGBLed(11, 9, 10);
	hueHandler = new RGBLedProgram::HueIntervalHandler(outLed);
	hueTimer.setDelay(0.01f);
	hueTimer.setHandler(hueHandler);
}

void RGBLedProgram::update(float dt)
{
	Program::update(dt);
	hueTimer.update(dt);
}

RGBLedProgram::HueIntervalHandler::HueIntervalHandler(RGBLed* iOutLed) : outLed(iOutLed) {}

void RGBLedProgram::HueIntervalHandler::onInterval()
{
	hue += 1.f;
	if (hue > 360.f)
	{
		hue -= 360.f;
	}
	outLed->setHSV(hue, 1.f, 1.f);
}