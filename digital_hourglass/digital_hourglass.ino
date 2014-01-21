#include <NewPing.h>
#include "DeltaTimer.h"
#include "DigitalSensor.h"
#include "LedGroup.h"
#include "Led.h"
#include "Timer.h"

const int NUM_LEDS = 6;

DeltaTimer deltaTimer;
DigitalSensor tiltSwitch(12);
LedGroup leds;
int nextLed;
Timer timer;
Timer blinkTimer;
bool blinkState;

void setup()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        Led* led = new Led(3 + i, true);
        led->off();
        leds.addLed(led);
    }
    timer.setDelay(60.f * 10.f); //10 minutes
    timer.reset();
    blinkTimer.setDelay(.25f);
    blinkTimer.reset();
    nextLed = 0;
    blinkState = false;
}

void loop()
{
    float dt = deltaTimer.updateDt();
    if (!tiltSwitch.getState())
    {
        timer.reset();
        blinkTimer.reset();
        leds.off();
        nextLed = 0;
        return;
    }
    if (nextLed < NUM_LEDS)
    {
        timer.update(dt);
        if (timer.isTriggered())
        {
            leds.get(nextLed)->on();
            nextLed++;
            timer.reset();
            if (nextLed >= NUM_LEDS)
            {
                blinkTimer.reset();
                blinkState = false;
                leds.off();
            }
        }
    }
    else
    {
        blinkTimer.update(dt);
        if (blinkTimer.isTriggered())
        {
            blinkTimer.reset();
            blinkState = !blinkState;
            if (blinkState)
            {
                leds.on();
            }
            else
            {
                leds.off();
            }
        }
    }
}

