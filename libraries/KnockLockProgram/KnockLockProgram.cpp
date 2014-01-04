#include "KnockLockProgram.h"
#include <Arduino.h>

void KnockLockProgram::init()
{
	Program::init();

	sensor = PiezoSensor(A0);
	sensor.setMax(100);

	servo = AbsoluteServo(9);
	servo.setPercent(0.f);

	button = DigitalSensor(2);

	progressLed = Led(11, true);
	progressLed.off();

	unlockedLed = Led(12, true);
	unlockedLed.off();

	lockedLed = Led(13, true);
	lockedLed.off();
}

void KnockLockProgram::update(float dt)
{
	Program::update(dt);
	float reading = sensor.getPercent();
	bool buttonState = button.getState();
}