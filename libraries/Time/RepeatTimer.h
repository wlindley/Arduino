#ifndef __REPEATTIMER_H__
#define __REPEATTIMER_H__

class IIntervalHandler
{
	public:
		virtual void onInterval() = 0;
};

class RepeatTimer
{
public:
	RepeatTimer();
	void setDelay(float seconds);
	void update(float dt);
	void setHandler(IIntervalHandler* iHandler);

private:
	float timer, delay;
	IIntervalHandler* handler;
};

#endif