#include "PiezoSensor.h"
#include <Arduino.h>

float PiezoSensor::getPercent()
{
	return min(max(float(getReading() - rangeMin) / float(rangeMax - rangeMin), 0.f), 1.f);
}

void PiezoSensor::setMin(int iMin)
{
	rangeMin = iMin;
}

void PiezoSensor::setMax(int iMax)
{
	rangeMax = iMax;
}