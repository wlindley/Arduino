#include <NewPing.h>
#include <Servo.h>
#include "PotSensor.h"
#include "Motor.h"

PotSensor pot(A0);
Motor motor(3, false);

void setup()
{
    
}

void loop()
{
    motor.setSpeed(pot.getPercent());
}
