#include "LedGroup.h"
#include <Arduino.h>

LedGroup::LedGroup() : Led()
{
	for (int i = 0; i < MAX_PINS; i++)
	{
		outputPins[i] = -1;
	}
}

bool LedGroup::addLed(int iPin)
{
	for (int i = 0; i < MAX_PINS; i++)
	{
		if (-1 == outputPins[i])
		{
			outputPins[i] = iPin;
			pinMode(iPin, OUTPUT);
			return true;
		}
	}
	return false;
}

void LedGroup::setIntensity(float iIntensity)
{
	int intensity = iIntensity * 255;
	for (int i = 0; i < MAX_PINS; i++)
	{
		if (-1 != outputPins[i])
		{
			analogWrite(outputPins[i], intensity);
		}
	}
}