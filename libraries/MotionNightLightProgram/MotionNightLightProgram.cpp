#include "MotionNightLightProgram.h"
#include <Arduino.h>
#include <math.h>
#include "Led.h"

const float LIGHT_THRESHOLD = .25f;
const float FADE_ON_SPEED = .5f;
const float FADE_OFF_SPEED = -.1f;
const float QUICK_FADE_OFF_SPEED = -1.f;
const float ON_TIME = 30.f;

void MotionNightLightProgram::init()
{
	photoSensor = PhotoSensor(A0);
	motionSensor = DigitalSensor(2);

	led = new Led(11, false);

	ledAnimator = LedAnimator(led);
	ledAnimator.off();

	onTimer.setDelay(ON_TIME);
}

void MotionNightLightProgram::update(float dt)
{
	onTimer.update(dt);
	ledAnimator.update(dt);

	if (photoSensor.getPercent() > LIGHT_THRESHOLD)
	{
		onTimer.clear();
		ledAnimator.setIntensityVelocity(QUICK_FADE_OFF_SPEED);
		if (0.001f >= abs(ledAnimator.getIntensityVelocity()))
		{
			delay(100);
		}
		return;
	}

	if (motionSensor.getState())
	{
		ledAnimator.setIntensityVelocity(FADE_ON_SPEED);
		onTimer.reset();
	}
	if (onTimer.isTriggered())
	{
		ledAnimator.setIntensityVelocity(FADE_OFF_SPEED);
	}
}