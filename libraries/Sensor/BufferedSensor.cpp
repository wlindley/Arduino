#include "BufferedSensor.h"
#include <Arduino.h>

int BufferedSensor::getReading()
{
	buffer.addSample(Sensor::getReading());
	return buffer.getAverage();
}