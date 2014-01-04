#include "KnockLockProgram.h"
#include <Arduino.h>

const float SERVO_UNLOCKED = 0.f;
const float SERVO_LOCKED = .5f;
const float KNOCK_TIME = 1.f;
const int KNOCK_RANGE_MAX = 100;
const float MIN_KNOCK_THRESHOLD = .2f;
const float MAX_KNOCK_THRESHOLD = .5f;
const int KNOCKS_TO_UNLOCK = 3;
const float MAX_INPUT_TIME = 5.f;

void KnockLockProgram::init()
{
	Program::init();

	sensor = PiezoSensor(A0);
	sensor.setMax(KNOCK_RANGE_MAX);

	servo = AbsoluteServo(9);
	servo.setPercent(SERVO_UNLOCKED);

	button = DigitalSensor(2);

	progressLed = Led(11, true);
	progressLed.off();

	unlockedLed = Led(12, true);
	unlockedLed.on();

	lockedLed = Led(13, true);
	lockedLed.off();

	knockTimer.setDelay(KNOCK_TIME);
	idleTimer.setDelay(MAX_INPUT_TIME);

	isLocked = false;
}

void KnockLockProgram::update(float dt)
{
	Program::update(dt);
	knockTimer.update(dt);

	if (button.getState())
	{
		lock();
	}
	checkForKnocks(dt);
}

void KnockLockProgram::lock()
{
	if (!isLocked)
	{
		servo.setPercent(SERVO_LOCKED);
		unlockedLed.off();
		lockedLed.on();
		numKnocks = -1; //the button is showing up as a knock
		isLocked = true;
	}
}

void KnockLockProgram::unlock()
{
	if (isLocked)
	{
		servo.setPercent(SERVO_UNLOCKED);
		unlockedLed.on();
		lockedLed.off();
		isLocked = false;
	}
}

void KnockLockProgram::checkForKnocks(float dt)
{
	if (isLocked)
	{
		if (knockTimer.isTriggered())
		{
			progressLed.off();
			idleTimer.update(dt);
			if (idleTimer.isTriggered())
			{
				numKnocks = 0;
				Serial.println("resetting knock count");
			}
			float knock = sensor.getPercent();
			if (knock < MAX_KNOCK_THRESHOLD && knock > MIN_KNOCK_THRESHOLD)
			{
				Serial.print("got knock: ");
				Serial.println(knock);
				numKnocks++;
				if (numKnocks >= KNOCKS_TO_UNLOCK)
				{
					Serial.println(" unlocking");
					progressLed.off();
					numKnocks = 0;
					unlock();
				}
				else
				{
					Serial.println(" waiting for another knock");
					progressLed.on();
					knockTimer.reset();
					idleTimer.reset();
				}
			}
		}
	}
}