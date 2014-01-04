#include "LedGroup.h"
#include <Arduino.h>

LedGroup::LedGroup() : Led()
{
	for (int i = 0; i < MAX_PINS; i++)
	{
		outputPins[i] = -1;
	}
}

LedGroup::LedGroup(bool iIsDigital) : Led(-1, isDigital)
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
	for (int i = 0; i < MAX_PINS; i++)
	{
		if (-1 != outputPins[i])
		{
			writeToPin(outputPins[i], iIntensity);
		}
	}
}