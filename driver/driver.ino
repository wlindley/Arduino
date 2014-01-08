#include <Servo.h>
#include <LiquidCrystal.h>
#include "Program.h"
#include "Led.h"
#include "Timer.h"
#include "Sensor.h"
#include "AbsoluteServo.h"
#include "PiezoSpeaker.h"
#include "LCD.h"

#include "DigitalHourglassProgram.h"

DigitalHourglassProgram program;

void setup()
{
  program.setup();
}

void loop()
{
  program.loop();
}

