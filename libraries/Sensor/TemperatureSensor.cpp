#include "TemperatureSensor.h"
#include <Arduino.h>

TemperatureSensor::TemperatureSensor() : Sensor() {}

TemperatureSensor::TemperatureSensor(int iPin) : Sensor(iPin) {}

float TemperatureSensor::getCelsius()
{
	if (0 > inputPin)
	{
		return 0.f;
	}
	float voltage = (getReading() / 1024.f) * 5.f;
	float temperature = (voltage - .5f) * 100.f;
	return temperature;
}