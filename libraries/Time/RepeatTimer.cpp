#include "RepeatTimer.h"
#include <Arduino.h>

RepeatTimer::RepeatTimer() : timer(0), delay(0), handler(NULL)
{

}

void RepeatTimer::setDelay(float iDelay)
{
	delay = iDelay;
}

void RepeatTimer::update(float dt)
{
	timer -= dt;
	while(0 >= timer)
	{
		if (handler)
		{
			handler->onInterval();
		}
		timer += delay;
	}
}

void RepeatTimer::setHandler(IIntervalHandler* iHandler)
{
	handler = iHandler;
}