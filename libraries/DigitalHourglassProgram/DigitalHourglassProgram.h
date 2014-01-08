#ifndef __DIGITALHOURGLASS_H__
#define __DIGITALHOURGLASS_H__

#include "Program.h"
#include "DigitalSensor.h"
#include "LedGroup.h"
#include "Timer.h"

class DigitalHourglassProgram : public Program
{
protected:
	virtual void init();
	virtual void update(float dt);

private:
	DigitalSensor tiltSwitch;
	LedGroup leds;
	int nextLed;
	Timer timer;
	Timer blinkTimer;
	bool blinkState;
};

#endif