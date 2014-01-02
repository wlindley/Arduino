#include "Program.h"
#include "RGBLed.h"
#include "Timer.h"
#include "TemperatureSensor.h"

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

