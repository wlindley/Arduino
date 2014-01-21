#include "RGBLed.h"

RGBLed outLed(11, 9, 10);
float hue = 0.f;

void setup()
{
    
}

void loop()
{
    hue += 1.f;
    if (hue >= 360.f)
    {
        hue -= 360.f;
    }
    outLed.setHSV(hue, 1.f, 1.f);
    delay(10);
};
