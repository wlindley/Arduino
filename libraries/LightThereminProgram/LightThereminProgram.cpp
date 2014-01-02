#include "LightThereminProgram.h"
#include <Arduino.h>

void LightThereminProgram::init()
{
	Program::init();
	sensor = PhotoSensor(A0);
	speaker = PiezoSpeaker(8);
}

void LightThereminProgram::update(float dt)
{
	Program::update(dt);

	float value = sensor.getPercent();
	Serial.println(value);
	speaker.playTone(value, .1);
	delay(10);
}