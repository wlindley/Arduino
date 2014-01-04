#include "DigitalSensor.h"
#include <Arduino.h>

DigitalSensor::DigitalSensor() : inputPin(-1)
{

}

DigitalSensor::DigitalSensor(int iPin) : inputPin(iPin)
{
	pinMode(iPin, INPUT);
}

bool DigitalSensor::getState()
{
	return 0 <= inputPin && HIGH == digitalRead(inputPin);
}