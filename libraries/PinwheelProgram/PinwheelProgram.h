#ifndef __PINWHEELPROGRAM_H__
#define __PINWHEELPROGRAM_H__

#include "Program.h"
#include "PotSensor.h"
#include "Motor.h"

class PinwheelProgram : public Program
{
protected:
	virtual void init();
	virtual void update(float dt);

private:
	PotSensor pot;
	Motor motor;
};

#endif