#ifndef __CRYSTALBALLPROGRAM_H__
#define __CRYSTALBALLPROGRAM_H__

#include "Program.h"
#include "DigitalSensor.h"
#include <LiquidCrystal.h>

class CrystalBallProgram : public Program
{
protected:
	virtual void init();
	virtual void update(float dt);

private:
	void showRandomAnswer();
	const char* getRandomAnswer();

	DigitalSensor tiltSwitch;
	LiquidCrystal* lcd;
	bool prevTiltState;
};

#endif