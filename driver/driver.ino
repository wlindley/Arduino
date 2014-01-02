#include "Program.h"
#include "RGBLed.h"
#include "Timer.h"
#include "Sensor.h"

#include "LedTempProgram.h"

LedTempProgram program;

void setup()
{
  program.setup();
}

void loop()
{
  program.loop();
}

