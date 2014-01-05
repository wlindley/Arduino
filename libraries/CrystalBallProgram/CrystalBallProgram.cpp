#include "CrystalBallProgram.h"
#include <Arduino.h>

void CrystalBallProgram::init()
{
	tiltSwitch = DigitalSensor(12);

	lcd = new LCD(2, 3, 6, 7, 9, 10);
	lcd->begin(16, 2);
	lcd->printLines("Ask the", "Crystal Ball!");

	prevTiltState = false;
}

void CrystalBallProgram::update(float dt)
{
	bool curTiltState = tiltSwitch.getState();
	if (!prevTiltState && curTiltState)
	{
		lcd->noDisplay();
	}
	else if (prevTiltState && !curTiltState)
	{
		lcd->display();
		showRandomAnswer();
	}
	prevTiltState = curTiltState;
}

void CrystalBallProgram::showRandomAnswer()
{
	lcd->printLines("The ball says:", getRandomAnswer());
}

const char* CrystalBallProgram::getRandomAnswer()
{
	switch(random(8))
	{
	case 0:
		return "Yes";
	case 1:
		return "Most likely";
	case 2:
		return "Certainly";
	case 3:
		return "Outlook good";
	case 4:
		return "Unsure";
	case 5:
		return "Ask again";
	case 6:
		return "Doubtful";
	case 7:
	default:
		return "No";
	}
}