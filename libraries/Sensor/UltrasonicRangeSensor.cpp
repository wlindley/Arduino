#include "UltrasonicRangeSensor.h"
#include <NewPing.h>

unsigned long UltrasonicRangeSensor::lastReadingTime = 0;

UltrasonicRangeSensor::UltrasonicRangeSensor()
{
	sensor = NULL;
}

UltrasonicRangeSensor::UltrasonicRangeSensor(int iTriggerPin, int iEchoPin, int iMaxDistance)
{
	sensor = new NewPing(iTriggerPin, iEchoPin, iMaxDistance * 100); //convert from meters to centimeters
	maxDistance = iMaxDistance;
}

UltrasonicRangeSensor::~UltrasonicRangeSensor()
{
	delete sensor;
}

float UltrasonicRangeSensor::getPercent()
{
	return getMeters() / (float)maxDistance;
}

float UltrasonicRangeSensor::getMeters()
{
	return (getMicroSeconds() / (float)US_ROUNDTRIP_CM) / 100.f;
}

unsigned int UltrasonicRangeSensor::getMicroSeconds()
{
	unsigned long curTime = millis();
	unsigned long delta = curTime - lastReadingTime;
	if (delta < MIN_READ_DELAY)
	{
		delay(MIN_READ_DELAY - delta);
	}
	lastReadingTime = millis();
	return (NULL == sensor) ? 0 : sensor->ping();
}