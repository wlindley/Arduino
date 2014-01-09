#ifndef __LEDANIMATOR_H__
#define __LEDANIMATOR_H__

#include "ILed.h"

class LedAnimator : ILed
{
public:
	LedAnimator();
	LedAnimator(ILed* iLed);
	void update(float dt);

	void setIntensityVelocity(float iIntensityVelocity);
	float getIntensityVelocity();

	//ILed Interface
	void setIntensity(float iIntensity);
	void on();
	void off();

protected:
	ILed* led;
	float intensity;
	float intensityVelocity;
};

#endif