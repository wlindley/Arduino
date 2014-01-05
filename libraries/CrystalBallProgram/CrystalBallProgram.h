#ifndef __CRYSTALBALLPROGRAM_H__
#define __CRYSTALBALLPROGRAM_H__

#include "Program.h"
#include "DigitalSensor.h"
#include "LCD.h"

class CrystalBallProgram : public Program
{
protected:
	virtual void init();
	virtual void update(float dt);

private:
	void showRandomAnswer();
	const char* getRandomAnswer();

	DigitalSensor tiltSwitch;
	LCD* lcd;
	bool prevTiltState;
};

#endif