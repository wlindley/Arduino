#include <NewPing.h>
#include "DeltaTimer.h"
#include "PhotoSensor.h"
#include "ILed.h"
#include "Led.h"
#include "Timer.h"
#include "DigitalSensor.h"
#include "LedAnimator.h"
#include <math.h>

const float LIGHT_THRESHOLD = .25f;
const float FADE_ON_SPEED = .5f;
const float FADE_OFF_SPEED = -.1f;
const float QUICK_FADE_OFF_SPEED = -1.f;
const float ON_TIME = 45.f;

DeltaTimer deltaTimer;
PhotoSensor photoSensor(A6);
DigitalSensor motionSensor(12);
ILed* led = new Led(11, false);
Timer onTimer;
LedAnimator ledAnimator(led);

void setup()
{
    ledAnimator.off();
    onTimer.setDelay(ON_TIME);
}

void loop()
{
    float dt = deltaTimer.updateDt();
    
    onTimer.update(dt);
    ledAnimator.update(dt);

    if (photoSensor.getPercent() > LIGHT_THRESHOLD)
    {
        onTimer.clear();
        ledAnimator.setIntensityVelocity(QUICK_FADE_OFF_SPEED);
        if (0.001f >= abs(ledAnimator.getIntensityVelocity()))
        {
            delay(100);
        }
        return;
    }

    if (motionSensor.getState())
    {
        ledAnimator.setIntensityVelocity(FADE_ON_SPEED);
        onTimer.reset();
    }
    if (onTimer.isTriggered())
    {
        ledAnimator.setIntensityVelocity(FADE_OFF_SPEED);
    }
}

