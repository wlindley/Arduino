#ifndef __LEDGROUP_H__
#define __LEDGROUP_H__

#include "ILed.h"

class LedGroup : public ILed
{
public:
	LedGroup();
	bool addLed(ILed* iLed);
	ILed* get(int iIndex);
	virtual void setIntensity(float iIntensity);
	virtual void on();
	virtual void off();

protected:
	static const int MAX_LEDS = 10;
	ILed* leds[MAX_LEDS];
};

#endif