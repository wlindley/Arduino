#ifndef __MOTIONNIGHTLIGHTPROGRAM_H__
#define __MOTIONNIGHTLIGHTPROGRAM_H__

#include "Program.h"
#include "PhotoSensor.h"
#include "ILed.h"
#include "Timer.h"
#include "DigitalSensor.h"
#include "LedAnimator.h"

class MotionNightLightProgram : public Program
{
protected:
	virtual void init();
	virtual void update(float dt);

private:
	PhotoSensor photoSensor;
	DigitalSensor motionSensor;
	ILed* led;
	Timer onTimer;
	LedAnimator ledAnimator;
};

#endif