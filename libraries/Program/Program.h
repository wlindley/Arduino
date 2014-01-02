#ifndef __PROGRAM_H__
#define __PROGRAM_H__

class Program
{
public:
	Program() : prevTime(0) {}
	void setup();
	void loop();

protected:
	virtual void init() {}
	virtual void update(float dt) {}
	
private:
	unsigned long prevTime;
};

#endif