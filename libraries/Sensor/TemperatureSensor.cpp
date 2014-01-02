#include "TemperatureSensor.h"
#include <Arduino.h>

TemperatureSensor::TemperatureSensor() : inputPin(-1) {}

TemperatureSensor::TemperatureSensor(int iPin) : inputPin(iPin)
{
	pinMode(inputPin, INPUT);
}

float TemperatureSensor::getCelsius()
{
	if (0 > inputPin)
	{
		return 0.f;
	}
	float voltage = (analogRead(inputPin) / 1024.f) * 5.f;
	float temperature = (voltage - .5f) * 100.f;
	return temperature;
}