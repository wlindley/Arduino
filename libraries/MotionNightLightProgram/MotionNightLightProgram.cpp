#include "MotionNightLightProgram.h"
#include <Arduino.h>

const float LIGHT_THRESHOLD = .25f;
const float VIBRATION_THRESHOLD = .05f;
const float MAX_VIBRATION = 50;
const float ON_TIME = 30.f;
const float FADE_TIME = 30.f;

void MotionNightLightProgram::init()
{
	vibrationSensor = PiezoSensor(A0);
	vibrationSensor.setMax(MAX_VIBRATION);
	photoSensor = PhotoSensor(A5);

	led.addLed(11);
	led.addLed(10);
	led.addLed(9);
	led.off();

	onTimer.setDelay(ON_TIME);
	fadeTimer.setDelay(FADE_TIME);

	ledState = STATE_OFF;
}

void MotionNightLightProgram::update(float dt)
{
	onTimer.update(dt);
	fadeTimer.update(dt);

	if (photoSensor.getPercent() > LIGHT_THRESHOLD)
	{
		onTimer.clear();
		fadeTimer.clear();
		led.off();
		return;
	}

	if (STATE_ON != ledState && vibrationSensor.getPercent() > VIBRATION_THRESHOLD)
	{
		Serial.println("detected vibration!");
		led.on();
		onTimer.reset();
		ledState = STATE_ON;
	}
	if (STATE_ON == ledState && onTimer.isTriggered())
	{
		Serial.println("switching to fade");
		fadeTimer.reset();
		ledState = STATE_FADE;
	}
	if (STATE_FADE == ledState)
	{
		Serial.println("fading");
		led.setIntensity(fadeTimer.getTimeRemaining() / fadeTimer.getDelay());
		if (fadeTimer.isTriggered())
		{
			Serial.println("turning off");
			led.off();
			ledState = STATE_OFF;
		}
	}
}