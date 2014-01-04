#ifndef __LED_H__
#define __LED_H__

class Led
{
public:
	Led();
	Led(int iPin, bool iIsDigital);
	virtual void setIntensity(float iItensity);
	virtual void on();
	virtual void off();

protected:
	int outputPin;
	bool isDigital;

	virtual void writeToPin(int iPinNumber, float iIntensity);
};

#endif