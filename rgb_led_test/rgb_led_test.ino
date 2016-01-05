#include "NewPing.h"
#include "RangedSensor.h"
#include "Led.h"

RangedSensor input(A5, 375, 525);
Led output(13, false);

void setup()
{
    Serial.begin(9600);
    output.on();
}

void loop()
{
    float p = input.getPercent();
    output.setIntensity(p);
    Serial.print("Percent: ");
    Serial.println(p);
    delay(50);
};
