#include "Led.h"
#include <Arduino.h>

Led::Led() : outputPin(-1) {}

Led::Led(int iPin) : outputPin(iPin)
{
	pinMode(iPin, OUTPUT);
}

void Led::setIntensity(float iItensity)
{
	if (0 > outputPin)
	{
		return;
	}
	analogWrite(outputPin, iItensity * 255);
}

void Led::on()
{
	setIntensity(1.f);
}

void Led::off()
{
	setIntensity(0.f);
}