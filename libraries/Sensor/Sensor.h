#ifndef __SENSOR_H__
#define __SENSOR_H__

class Sensor
{
public:
	Sensor();
	Sensor(int iPin);
	virtual int getReading();

protected:
	static const unsigned long MIN_READ_DELAY = 1; //milliseconds
	static unsigned long lastReadingTime;
	int inputPin;
};

#endif