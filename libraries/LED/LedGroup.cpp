#include "LedGroup.h"
#include <Arduino.h>

LedGroup::LedGroup()
{
	for (int i = 0; i < MAX_LEDS; i++)
	{
		leds[i] = NULL;
	}
}

bool LedGroup::addLed(ILed* iLed)
{
	for (int i = 0; i < MAX_LEDS; i++)
	{
		if (NULL == leds[i])
		{
			leds[i] = iLed;
			return true;
		}
	}
	return false;
}

ILed* LedGroup::get(int iIndex)
{
	if (iIndex < 0 || iIndex >= MAX_LEDS || NULL == leds[iIndex])
	{
		return NULL;
	}
	return leds[iIndex];
}

void LedGroup::setIntensity(float iIntensity)
{
	for (int i = 0; i < MAX_LEDS; i++)
	{
		if (NULL != leds[i])
		{
			leds[i]->setIntensity(iIntensity);
		}
	}
}

void LedGroup::on()
{
	for (int i = 0; i < MAX_LEDS; i++)
	{
		if (NULL != leds[i])
		{
			leds[i]->on();
		}
	}
}

void LedGroup::off()
{
	for (int i = 0; i < MAX_LEDS; i++)
	{
		if (NULL != leds[i])
		{
			leds[i]->off();
		}
	}
}