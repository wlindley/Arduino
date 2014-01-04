#ifndef __KNOCKLOCKPROGRAM_H__
#define __KNOCKLOCKPROGRAM_H__

#include "Program.h"
#include "PiezoSensor.h"
#include "Led.h"
#include "AbsoluteServo.h"
#include "DigitalSensor.h"
#include "Timer.h"

class KnockLockProgram : public Program
{
protected:
	virtual void init();
	virtual void update(float dt);

private:
	void lock();
	void unlock();
	void checkForKnocks(float dt);

	PiezoSensor sensor;
	Led progressLed;
	Led unlockedLed;
	Led lockedLed;
	AbsoluteServo servo;
	DigitalSensor button;
	Timer knockTimer;
	Timer idleTimer;
	bool isLocked;
	int numKnocks;
};

#endif