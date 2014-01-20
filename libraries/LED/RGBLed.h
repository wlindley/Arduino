#ifndef __RGBLED_H__
#define __RGBLED_H__

#include "ILed.h"

union Color
{
	unsigned int color;
	struct
	{
		unsigned char a, r, g, b;
	};
};

Color hsvToRGB(float hue, float saturation, float value);

class RGBLed : public ILed
{
public:
	RGBLed();
	RGBLed(int iRedPin, int iGreenPin, int iBluePin);

	virtual void setIntensity(float iIntensity);
	virtual void on();
	virtual void off();
	
	void setRGB(int red, int green, int blue);
	void setRGB(float red, float green, float blue);
	void setHSV(float hue, float saturation, float value);
	
private:
	int redPin, greenPin, bluePin;
	Color currentColor;
	float intensity;

	void updateLed();
};

#endif