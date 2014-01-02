#include "PhotoSensor.h"

PhotoSensor::PhotoSensor() : BufferedSensor()
{
	min = 0;
	max = 1023;
}

PhotoSensor::PhotoSensor(int iPin) : BufferedSensor(iPin)
{
	min = 0;
	max = 1023;
}

float PhotoSensor::getPercent()
{
	return float(getReading() - min) / float(max - min);
}

void PhotoSensor::setMin(int iMin)
{
	min = iMin;
}

void PhotoSensor::setMax(int iMax)
{
	max = iMax;
}