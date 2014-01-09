#include "Led.h"
#include <Arduino.h>

Led::Led() : outputPin(-1), isDigital(true) {}

Led::Led(int iPin, bool iIsDigital) : outputPin(iPin), isDigital(iIsDigital)
{
	if (0 <= iPin)
	{
		pinMode(iPin, OUTPUT);
	}
}

void Led::setIntensity(float iIntensity)
{
	if (0 > outputPin)
	{
		return;
	}
	writeToPin(outputPin, iIntensity);
}

void Led::on()
{
	setIntensity(1.f);
}

void Led::off()
{
	setIntensity(0.f);
}

void Led::writeToPin(int iPinNumber, float iIntensity)
{
	iIntensity = constrain(iIntensity, 0.f, 1.f);
	if (isDigital)
	{
		digitalWrite(iPinNumber, iIntensity >= .5f ? HIGH : LOW);
	}
	else
	{
		analogWrite(iPinNumber, iIntensity * 255);
	}
}