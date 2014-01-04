#include <Servo.h>
#include "Program.h"
#include "RGBLed.h"
#include "Timer.h"
#include "Sensor.h"
#include "AbsoluteServo.h"
#include "PiezoSpeaker.h"

#include "KnockLockProgram.h"

KnockLockProgram program;

void setup()
{
  program.setup();
}

void loop()
{
  program.loop();
}

