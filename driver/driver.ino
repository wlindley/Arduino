#include <Servo.h>
#include "Program.h"
#include "RGBLed.h"
#include "Timer.h"
#include "Sensor.h"

#include "MoodCueProgram.h"

MoodCueProgram program;

void setup()
{
  program.setup();
}

void loop()
{
  program.loop();
}

