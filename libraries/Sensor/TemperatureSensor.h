#ifndef __TEMPERATURESENSOR_H__
#define __TEMPERATURESENSOR_H__

class TemperatureSensor
{
	public:
		TemperatureSensor();
		TemperatureSensor(int iPin);
		float getCelsius();
		
	private:
		int inputPin;
};

#endif