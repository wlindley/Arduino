#ifndef __MOTOR_H__
#define __MOTOR_H__

class Motor
{
public:
	Motor();
	Motor(int iPin, bool iIsDigital);
	void on();
	void off();
	void setSpeed(float iPercent);

protected:
	int outputPin;
	bool isDigital;
};

#endif