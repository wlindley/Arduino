#ifndef __TIMER_H__
#define __TIMER_H__

class Timer
{
public:
	Timer();
	void setDelay(float seconds);
	void reset();
	void update(float dt);
	bool isTriggered();

private:
	float timer, delay;
};

#endif