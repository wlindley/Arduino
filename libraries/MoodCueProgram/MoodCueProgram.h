#ifndef __MOODCUEPROGRAM_H__
#define __MOODCUEPROGRAM_H__

#include "Program.h"
#include "AbsoluteServo.h"
#include "PotSensor.h"

class MoodCueProgram : public Program
{
protected:
	virtual void init();
	virtual void update(float dt);

private:
	AbsoluteServo servo;
	PotSensor sensor;
};

#endif