#include "MotionNightLightProgram.h"
#include <Arduino.h>
#include "Led.h"

const float LIGHT_THRESHOLD = .25f;
const float ON_TIME = 30.f;
const float FADE_TIME = 30.f;

void MotionNightLightProgram::init()
{
	photoSensor = PhotoSensor(A0);
	motionSensor = DigitalSensor(2);

	led.addLed(new Led(11, false));
	led.addLed(new Led(13, true));
	led.off();

	onTimer.setDelay(ON_TIME);
	fadeTimer.setDelay(FADE_TIME);

	ledState = STATE_OFF;
}

void MotionNightLightProgram::update(float dt)
{
	onTimer.update(dt);
	fadeTimer.update(dt);

	Serial.println(motionSensor.getState());

	if (photoSensor.getPercent() > LIGHT_THRESHOLD)
	{
		onTimer.clear();
		fadeTimer.clear();
		led.off();
		ledState = STATE_OFF;
		return;
	}

	if (STATE_ON != ledState && motionSensor.getState())
	{
		//Serial.println("detected vibration!");
		led.on();
		onTimer.reset();
		ledState = STATE_ON;
	}
	if (STATE_ON == ledState && onTimer.isTriggered())
	{
		//Serial.println("switching to fade");
		fadeTimer.reset();
		ledState = STATE_FADE;
	}
	if (STATE_FADE == ledState)
	{
		//Serial.println("fading");
		led.setIntensity(fadeTimer.getTimeRemaining() / fadeTimer.getDelay());
		if (fadeTimer.isTriggered())
		{
			//Serial.println("turning off");
			led.off();
			ledState = STATE_OFF;
		}
	}
}