#include <LiquidCrystal.h>
#include <NewPing.h>
#include "DigitalSensor.h"
#include "LCD.h"
#include "DeltaTimer.h"

DigitalSensor tiltSwitch(12);
LCD lcd(2, 3, 6, 7, 9, 10);
bool prevTiltState = false;
DeltaTimer deltaTimer;

void setup()
{
    lcd.begin(16, 2);
    lcd.printLines("Ask the", "Crystal Ball!");
}

void loop()
{
    float dt = deltaTimer.updateDt();
    bool curTiltState = tiltSwitch.getState();
    if (!prevTiltState && curTiltState)
    {
        lcd.noDisplay();
    }
    else if (prevTiltState && !curTiltState)
    {
        lcd.display();
        showRandomAnswer();
    }
    prevTiltState = curTiltState;
}

void showRandomAnswer()
{
    lcd.printLines("The ball says:", getRandomAnswer());
}

const char* getRandomAnswer()
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
