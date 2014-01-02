#include "KnockLockProgram.h"
#include <Arduino.h>

void KnockLockProgram::init()
{
	Program::init();
	sensor = PiezoSensor(A0);
	sensor.setMax(100);
}

void KnockLockProgram::update(float dt)
{
	Program::update(dt);
	float reading = sensor.getPercent();
	if (reading > 0)
	{
		Serial.println(reading);
	}
}