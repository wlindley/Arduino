#include "TemperatureSensor.h"
#include <Arduino.h>

float TemperatureSensor::getCelsius()
{
	float voltage = (getReading() / 1024.f) * 5.f;
	float temperature = (voltage - .5f) * 100.f;
	return temperature;
}