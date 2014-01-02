#ifndef __TEMPERATURESENSOR_H__
#define __TEMPERATURESENSOR_H__

#include "Sensor.h"

class TemperatureSensor : public Sensor
{
public:
	TemperatureSensor() : Sensor() {}
	TemperatureSensor(int iPin) : Sensor(iPin) {}
	float getCelsius();
};

#endif