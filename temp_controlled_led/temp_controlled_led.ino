#include <NewPing.h>
#include "DeltaTimer.h"
#include "Timer.h"
#include "RGBLed.h"
#include "TemperatureSensor.h"

DeltaTimer deltaTimer;
RGBLed outLed(11, 9, 10);
TemperatureSensor tempSensor(A0);
FloatSampleBuffer sampleBuffer;
float initialReading;

void setup()
{
    initialReading = tempSensor.getCelsius();
    Serial.begin(9600);
    Serial.print("Initial temp: ");
    Serial.println(initialReading);
    delay(1);
}

void loop()
{
    float dt = deltaTimer.updateDt();
    sampleBuffer.addSample(tempSensor.getCelsius());
    float temp = sampleBuffer.getAverage();
    float hue = ((temp - initialReading) / 6.f) * 360.f;

    Serial.print("temp: ");
    Serial.print(temp);
    Serial.print(", hue: ");
    Serial.println(hue);

    outLed.setHSV(hue, 1.f, 1.f);
}

