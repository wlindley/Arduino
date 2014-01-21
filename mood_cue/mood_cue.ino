#include <NewPing.h>
#include <Servo.h>
#include "AbsoluteServo.h"
#include "PotSensor.h"

AbsoluteServo servo(9);
PotSensor sensor(A0);

void setup()
{
    
}

void loop()
{
    servo.setPercent(sensor.getPercent());
}
