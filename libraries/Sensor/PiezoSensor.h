#ifndef __PIEZOSENSOR_H__
#define __PIEZOSENSOR_H__

#include "BufferedSensor.h"

class PiezoSensor : public BufferedSensor
{
public:
	PiezoSensor() : BufferedSensor(), rangeMin(0), rangeMax(1023) {}
	PiezoSensor(int iPin) : BufferedSensor(iPin), rangeMin(0), rangeMax(1023) {}
	float getPercent();
	void setMin(int iMin);
	void setMax(int iMax);

private:
	int rangeMin, rangeMax;
};

#endif