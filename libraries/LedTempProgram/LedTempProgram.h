#ifndef __LEDTEMPPROGRAM_H__
#define __LEDTEMPPROGRAM_H__

#include "Program.h"
#include "RGBLed.h"
#include "Timer.h"
#include "TemperatureSensor.h"
#include "SampleBuffer.h"

class LedTempProgram : public Program
{
	protected:
		virtual void init();
		virtual void update(float dt);
	
	private:
		RGBLed outLed;
		Timer inputTimer;
		TemperatureSensor tempSensor;
		SampleBuffer sampleBuffer;
		float initialReading;
};

#endif