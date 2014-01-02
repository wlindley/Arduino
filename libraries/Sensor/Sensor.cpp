#include "Sensor.h"
#include <Arduino.h>

unsigned long Sensor::lastReadingTime = 0;

Sensor::Sensor()
{
	inputPin = -1;
}

Sensor::Sensor(int iPin) : inputPin(iPin)
{
	pinMode(inputPin, INPUT);
}

int Sensor::getReading()
{
	if (0 > inputPin)
	{
		return 0;
	}
	unsigned long curTime = millis();
	unsigned long delta = curTime - lastReadingTime;
	if (delta < MIN_READ_DELAY)
	{
		delay(MIN_READ_DELAY - delta);
	}
	lastReadingTime = millis();
	return analogRead(inputPin);
}