#ifndef __RGBLEDPROGRAM_H__
#define __RGBLEDPROGRAM_H__

#include "Program.h"
#include "RGBLed.h"
#include "Timer.h"

class RGBLedTestProgram : public Program
{
protected:
	virtual void init();
	virtual void update(float dt);

private:
	void updateHue(float dt);
	void updateValue(float dt);

	float hue;
	float value, valueDelta;
	RGBLed outLed;
	Timer hueTimer;
	Timer valueTimer;
};

#endif