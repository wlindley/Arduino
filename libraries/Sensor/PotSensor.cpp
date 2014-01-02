#include "PotSensor.h"
#include <Arduino.h>

#define PI 3.141592653589793238462643383f
#define DOUBLE_PI (2.f * PI)

float PotSensor::getPercent()
{
	return getReading() / 1023.f;
}

float PotSensor::getDegrees()
{
	return getPercent() * 360.f;
}

float PotSensor::getRadians()
{
	return getPercent() * DOUBLE_PI;
}