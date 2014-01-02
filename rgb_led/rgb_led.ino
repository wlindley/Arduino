#include "Program.h"
#include "RGBLed.h"
#include "RepeatTimer.h"
#include "RGBLedProgram.h"

RGBLedProgram program;

void setup()
{
  program.setup();
}

void loop()
{
  program.loop();
}

