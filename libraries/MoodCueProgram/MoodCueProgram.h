#ifndef __MOODCUEPROGRAM_H__
#define __MOODCUEPROGRAM_H__

#include "Program.h"
#include <Servo.h>

class MoodCueProgram : public Program
{
protected:
	virtual void init();
	virtual void update(float dt);

private:
	Servo servo;
	int pos;
	int speed;
};

#endif