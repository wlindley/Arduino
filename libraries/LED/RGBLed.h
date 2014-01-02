#ifndef __RGBLED_H__
#define __RGBLED_H__

struct Color
{
	char a, r, g, b;
};

Color hsvToRGB(float hue, float saturation, float value);

class RGBLed {
public:
	RGBLed();
	RGBLed(int iRedPin, int iGreenPin, int iBluePin);
	
	void setRGB(int red, int green, int blue);
	void setRGB(float red, float green, float blue);
	void setHSV(float hue, float saturation, float value);
	
private:
	int redPin, greenPin, bluePin;
};

#endif