#ifndef __LED_H__
#define __LED_H__

class Led
{
public:
	Led();
	Led(int iPin);
	virtual void setIntensity(float iItensity);
	virtual void on();
	virtual void off();

protected:
	int outputPin;
};

#endif