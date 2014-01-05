#include "PinwheelProgram.h"
#include <Arduino.h>

void PinwheelProgram::init()
{
	pot = PotSensor(A0);
	motor = Motor(3, false);
}

void PinwheelProgram::update(float dt)
{}
	motor.setSpeed(pot.getPercent());
}