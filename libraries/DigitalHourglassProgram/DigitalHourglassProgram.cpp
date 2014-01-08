#include "DigitalHourglassProgram.h"
#include <Arduino.h>
#include "Led.h"

const int NUM_LEDS = 6;

void DigitalHourglassProgram::init()
{
	tiltSwitch = DigitalSensor(12);
	for (int i = 0; i < NUM_LEDS; i++)
	{
		Led* led = new Led(3 + i, true);
		led->off();
		leds.addLed(led);
	}
	timer.setDelay(60.f * 10.f); //10 minutes
	timer.reset();
	blinkTimer.setDelay(.25f);
	blinkTimer.reset();
	nextLed = 0;
	blinkState = false;
}

void DigitalHourglassProgram::update(float dt)
{
	if (!tiltSwitch.getState())
	{
		timer.reset();
		blinkTimer.reset();
		leds.off();
		nextLed = 0;
		return;
	}
	if (nextLed < NUM_LEDS)
	{
		timer.update(dt);
		if (timer.isTriggered())
		{
			leds.get(nextLed)->on();
			nextLed++;
			timer.reset();
			if (nextLed >= NUM_LEDS)
			{
				blinkTimer.reset();
				blinkState = false;
				leds.off();
			}
		}
	}
	else
	{
		blinkTimer.update(dt);
		if (blinkTimer.isTriggered())
		{
			blinkTimer.reset();
			blinkState = !blinkState;
			if (blinkState)
			{
				leds.on();
			}
			else
			{
				leds.off();
			}
		}
	}
}