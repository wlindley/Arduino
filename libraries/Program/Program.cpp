#include "Program.h"
#include <Arduino.h>

void Program::setup()
{
	Serial.begin(9600);
	prevTime = millis();
	init();
}

void Program::loop()
{
	unsigned long curTime = millis();
	update((curTime - prevTime) / 1000.f);
	prevTime = curTime;
}