#include "LedAnimator.h"
#include <Arduino.h>

LedAnimator::LedAnimator() : led(NULL), intensity(0.f), intensityVelocity(0.f) { }

LedAnimator::LedAnimator(ILed* iLed) : led(iLed), intensity(0.f), intensityVelocity(0.f) { }

void LedAnimator::update(float dt)
{
	float newIntensity = constrain(intensity + (dt * intensityVelocity), 0.f, 1.f);
	if (0.f >= newIntensity || 1.f <= newIntensity)
	{
		intensityVelocity = 0.f;
	}
	setIntensity(newIntensity);
}

void LedAnimator::setIntensityVelocity(float iIntensityVelocity)
{
	intensityVelocity = iIntensityVelocity;
}

float LedAnimator::getIntensityVelocity()
{
	return intensityVelocity;
}

void LedAnimator::setIntensity(float iIntensity)
{
	intensity = iIntensity;
	if (NULL != led)
	{
		led->setIntensity(iIntensity);
	}
}

void LedAnimator::on()
{
	setIntensity(1.f);
}

void LedAnimator::off()
{
	setIntensity(0.f);
}