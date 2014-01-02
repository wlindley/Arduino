#ifndef __POTSENSOR_H__
#define __POTSENSOR_H__

#include "Sensor.h"

class PotSensor : public Sensor
{
public:
	PotSensor() : Sensor() {}
	PotSensor(int iPin) : Sensor(iPin) {}
	float getPercent();
	float getDegrees();
	float getRadians();
};

#endif