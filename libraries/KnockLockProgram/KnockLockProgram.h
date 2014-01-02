#ifndef __KNOCKLOCKPROGRAM_H__
#define __KNOCKLOCKPROGRAM_H__

#include "Program.h"
#include "PiezoSensor.h"

class KnockLockProgram : public Program
{
protected:
	virtual void init();
	virtual void update(float dt);

private:
	PiezoSensor sensor;
};

#endif