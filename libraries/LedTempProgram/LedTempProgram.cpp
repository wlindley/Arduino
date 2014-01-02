#include "LedTempProgram.h"
#include <Arduino.h>

void LedTempProgram::init()
{
	Program::init();

	outLed = RGBLed(11, 9, 10);
	tempSensor = TemperatureSensor(A0);
	initialReading = tempSensor.getCelsius();
	Serial.print("Initial temp: ");
	Serial.println(initialReading);
	delay(1);
}

void LedTempProgram::update(float dt)
{
	Program::update(dt);
	
	sampleBuffer.addSample(tempSensor.getCelsius());
	float temp = sampleBuffer.getAverage();
	//float hue = map(temp, initialReading, initialReading + 6.f, 0.f, 360.f);
	float hue = ((temp - initialReading) / 6.f) * 360.f;
	
	Serial.print("temp: ");
	Serial.print(temp);
	Serial.print(", hue: ");
	Serial.println(hue);
	
	outLed.setHSV(hue, 1.f, 1.f);
}