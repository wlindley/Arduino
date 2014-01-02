#ifndef __ABSOLUTESERVO_H__
#define __ABSOLUTESERVO_H__

#include <Servo.h>

class AbsoluteServo
{
public:
	AbsoluteServo();
	AbsoluteServo(int iPin);
	void setPercent(float iPercent);

private:
	int outputPin;
	int prevPos;
	Servo servo;
};

#endif