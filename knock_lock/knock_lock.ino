#include <NewPing.h>
#include <Servo.h>
#include "DeltaTimer.h"
#include "PiezoSensor.h"
#include "Led.h"
#include "AbsoluteServo.h"
#include "DigitalSensor.h"
#include "Timer.h"

const float SERVO_UNLOCKED = 0.f;
const float SERVO_LOCKED = .5f;
const float KNOCK_TIME = 1.f;
const int KNOCK_RANGE_MAX = 100;
const float MIN_KNOCK_THRESHOLD = .2f;
const float MAX_KNOCK_THRESHOLD = .5f;
const int KNOCKS_TO_UNLOCK = 3;
const float MAX_INPUT_TIME = 5.f;

DeltaTimer deltaTimer;
PiezoSensor sensor(A0);
Led progressLed(11, true);
Led unlockedLed(12, true);
Led lockedLed(13, true);
AbsoluteServo servo(9);
DigitalSensor button(2);
Timer knockTimer;
Timer idleTimer;
bool isLocked = false;
int numKnocks = 0;

void setup()
{
    sensor.setMax(KNOCK_RANGE_MAX);
    servo.setPercent(SERVO_UNLOCKED);
    progressLed.off();
    unlockedLed.on();
    lockedLed.off();
    knockTimer.setDelay(KNOCK_TIME);
    idleTimer.setDelay(MAX_INPUT_TIME);
}

void loop()
{
    float dt = deltaTimer.updateDt();
    knockTimer.update(dt);

    if (button.getState())
    {
        lock();
    }
    checkForKnocks(dt);
}

void lock()
{
    if (!isLocked)
    {
        servo.setPercent(SERVO_LOCKED);
        unlockedLed.off();
        lockedLed.on();
        numKnocks = -1; //the button is showing up as a knock
        isLocked = true;
    }
}

void unlock()
{
    if (isLocked)
    {
        servo.setPercent(SERVO_UNLOCKED);
        unlockedLed.on();
        lockedLed.off();
        isLocked = false;
    }
}

void checkForKnocks(float dt)
{
    if (isLocked)
    {
        if (knockTimer.isTriggered())
        {
            progressLed.off();
            idleTimer.update(dt);
            if (idleTimer.isTriggered())
            {
                numKnocks = 0;
                Serial.println("resetting knock count");
            }
            float knock = sensor.getPercent();
            if (knock < MAX_KNOCK_THRESHOLD && knock > MIN_KNOCK_THRESHOLD)
            {
                Serial.print("got knock: ");
                Serial.println(knock);
                numKnocks++;
                if (numKnocks >= KNOCKS_TO_UNLOCK)
                {
                    Serial.println(" unlocking");
                    progressLed.off();
                    numKnocks = 0;
                    unlock();
                }
                else
                {
                    Serial.println(" waiting for another knock");
                    progressLed.on();
                    knockTimer.reset();
                    idleTimer.reset();
                }
            }
        }
    }
}


