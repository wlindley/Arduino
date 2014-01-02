#include "MoodCueProgram.h"
#include <Arduino.h>

void MoodCueProgram::init()
{
	Program::init();

	sensor = PotSensor(A0);
	servo = AbsoluteServo(9);
}

void MoodCueProgram::update(float dt)
{
	Program::update(dt);
	
	servo.setPercent(sensor.getPercent());
}