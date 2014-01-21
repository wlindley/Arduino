#include <NewPing.h>
#include <NewTone.h>
#include "PhotoSensor.h"
#include "PiezoSpeaker.h"

PhotoSensor sensor(A0);
PiezoSpeaker speaker(8);

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    float value = sensor.getPercent();
    Serial.println(value);
    speaker.playTone(value, .1);
    delay(10);
}

