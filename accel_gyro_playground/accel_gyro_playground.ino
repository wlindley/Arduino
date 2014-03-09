#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "SampleBuffer.h"
#include "NewPing.h"

const int LED_PIN = 13;
const int NUM_SAMPLES = 1000;

#define PRINT_AVERAGE

MPU6050 sensor;
IntSampleBuffer buffer;
int16_t ax, ay, az, gx, gy, gz, temp;
float axOffset, ayOffset, azOffset, gxOffset, gyOffset, gzOffset;

void setup() {
    Serial.begin(9600);
    sensor.initialize();
    Serial.println(sensor.testConnection() ? "Connection successful" : "Connection failed");
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
}

void loop() {
    axOffset = 0;
    ayOffset = 0;
    azOffset = 0;
    gxOffset = 0;
    gyOffset = 0;
    gzOffset = 0;
    for (int i = 0; i < NUM_SAMPLES; i++) {
        sensor.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        axOffset += ax;
        ayOffset += ay;
        azOffset += az;
        gxOffset += gx;
        gyOffset += gy;
        gzOffset += gz;
        
        #ifdef PRINT_SAMPLES
        Serial.print(ax); Serial.print("\t");
        Serial.print(ay); Serial.print("\t");
        Serial.print(az); Serial.print("\t");
        Serial.print("\t-\t");
        Serial.print(gx); Serial.print("\t");
        Serial.print(gy); Serial.print("\t");
        Serial.print(gz); Serial.println("\t");
        #endif
        
        delay(50);
    }
    axOffset /= NUM_SAMPLES;
    ayOffset /= NUM_SAMPLES;
    azOffset /= NUM_SAMPLES;
    gxOffset /= NUM_SAMPLES;
    gyOffset /= NUM_SAMPLES;
    gzOffset /= NUM_SAMPLES;
    
    #ifdef PRINT_AVERAGE
    Serial.print(axOffset); Serial.print("\t");
    Serial.print(ayOffset); Serial.print("\t");
    Serial.print(azOffset); Serial.print("\t");
    Serial.print("\t-\t");
    Serial.print(gxOffset); Serial.print("\t");
    Serial.print(gyOffset); Serial.print("\t");
    Serial.print(gzOffset); Serial.print("\t");
    Serial.println(" - AVERAGE");
    #endif
}
