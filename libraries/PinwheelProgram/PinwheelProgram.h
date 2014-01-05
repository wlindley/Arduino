#ifndef __PINWHEELPROGRAM_H__
#define __PINWHEELPROGRAM_H__

#include "Program.h"
#include "DigitalSensor.h"

class PinwheelProgram : public Program
{
protected:
	virtual void init();
	virtual void update(float dt);

private:
	DigitalSensor button;
};

#endif