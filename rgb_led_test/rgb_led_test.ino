#include "RGBLed.h"

RGBLed outLed(3, 5, 6);
float hue = 0.f;

void setup()
{
    outLed.setIntensity(1.f);
}

void loop()
{
    hue += 1.f;
    if (hue >= 360.f)
    {
        hue -= 360.f;
    }
    outLed.setHSV(hue, 1.f, 1.f);
    delay(100);
};
