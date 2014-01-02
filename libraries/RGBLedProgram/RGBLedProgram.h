#ifndef __RGBLEDPROGRAM_H__
#define __RGBLEDPROGRAM_H__

#include "Program.h"
#include "RGBLed.h"
#include "RepeatTimer.h"

class RGBLedProgram : public Program
{
protected:
	virtual void init();
	virtual void update(float dt);

private:
	RGBLed* outLed;
	RepeatTimer hueTimer;
	
	class HueIntervalHandler : public IIntervalHandler
	{
		public:
			HueIntervalHandler(RGBLed* iOutLed);
			virtual void onInterval();
		
		private:
			RGBLed* outLed;
			float hue;
	};
	HueIntervalHandler* hueHandler;
};

#endif