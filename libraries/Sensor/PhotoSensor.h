#ifndef __PHOTOSENSOR_H__
#define __PHOTOSENSOR_H__

#include "BufferedSensor.h"

class PhotoSensor : public BufferedSensor
{
public:
	PhotoSensor();
	PhotoSensor(int iPin);
	float getPercent();
	void setMin(int iMin);
	void setMax(int iMax);

private:
	int min, max;
};

#endif