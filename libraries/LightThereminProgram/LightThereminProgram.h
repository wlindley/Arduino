#ifndef __LIGHTTHEREMINPROGRAM_H__
#define __LIGHTTHEREMINPROGRAM_H__

#include "Program.h"
#include "PhotoSensor.h"
#include "PiezoSpeaker.h"

class LightThereminProgram : public Program
{
protected:
	virtual void init();
	virtual void update(float dt);

private:
	PhotoSensor sensor;
	PiezoSpeaker speaker;
};

#endif