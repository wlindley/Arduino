#ifndef __MOTIONNIGHTLIGHTPROGRAM_H__
#define __MOTIONNIGHTLIGHTPROGRAM_H__

#include "Program.h"
#include "PhotoSensor.h"
#include "LedGroup.h"
#include "Timer.h"
#include "DigitalSensor.h"

class MotionNightLightProgram : public Program
{
protected:
	virtual void init();
	virtual void update(float dt);

private:
	PhotoSensor photoSensor;
	DigitalSensor motionSensor;
	LedGroup led;
	Timer onTimer;
	Timer fadeTimer;

	enum LedState {
		STATE_OFF,
		STATE_ON,
		STATE_FADE
	} ledState;
};

#endif