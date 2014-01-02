#ifndef __TEMPERATURESENSOR_H__
#define __TEMPERATURESENSOR_H__

#include "Sensor.h"

class TemperatureSensor : public Sensor
{
public:
	TemperatureSensor();
	TemperatureSensor(int iPin);
	float getCelsius();
	
private:
	int inputPin;
};

#endif