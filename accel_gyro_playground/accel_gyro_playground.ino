#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "SampleBuffer.h"
#include "NewPing.h"

const int LED_PIN = 13;
const int NUM_SAMPLES = 1000;
const int DELAY = 20;

#define PRINT_AVERAGE

MPU6050 sensor;
IntSampleBuffer buffer;
int16_t ax, ay, az, gx, gy, gz, temp;
int16_t axOffset, ayOffset, azOffset, gxOffset, gyOffset, gzOffset;
int16_t prevAZ;
float pos, vel, acc;

void setup() {
    Serial.begin(9600);
    sensor.initialize();
    Serial.println(sensor.testConnection() ? "Connection successful" : "Connection failed");
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
    axOffset = 620;
    ayOffset = 165;
    azOffset = -17495 & 0xFF80;
    gxOffset = 256;
    gyOffset = 39;
    gzOffset = 90;
    
    pos = 0;
    vel = 0;
    acc = 0;
    prevAZ = getZAccel();
    delay(DELAY);
}

void loop() {
    az = getZAccel();
    float dt = DELAY / 1000.f;
    int dAcc = az - prevAZ;
    acc = dAcc * dt;
    vel += acc * dt;
    pos += vel * dt;
    Serial.print(acc); Serial.print("\t");
    Serial.print(vel); Serial.print("\t");
    Serial.print(pos); Serial.println(" ");
    prevAZ = az;
    /*Serial.print(ax + axOffset); Serial.print("\t");
    Serial.print(ay + ayOffset); Serial.print("\t");
    Serial.print(az + azOffset); Serial.print("\t");
    Serial.print("\t-\t");
    Serial.print(gx + gxOffset); Serial.print("\t");
    Serial.print(gy + gyOffset); Serial.print("\t");
    Serial.print(gz + gzOffset); Serial.print("\t");
    Serial.print("\t-\t");
    Serial.println(temp);*/
    delay(DELAY);
}

int16_t getZAccel() {
    return (sensor.getAccelerationZ() - azOffset) & 0xFF80;
}
