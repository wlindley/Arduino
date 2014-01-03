#ifndef __LEDGROUP_H__
#define __LEDGROUP_H__

#include "Led.h"

class LedGroup : public Led
{
public:
	LedGroup();
	bool addLed(int iPin);
	virtual void setIntensity(float iIntensity);

protected:
	static const int MAX_PINS = 10;
	int outputPins[MAX_PINS];
};

#endif