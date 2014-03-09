#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"

const int LED_PIN = 13;
const int MAX_VALUE = 32768;

MPU6050 sensor;
int16_t ax, ay, az, gx, gy, gz, temp;

void setup() {
    Serial.begin(9600);
    sensor.initialize();
    Serial.println(sensor.testConnection() ? "Connection successful" : "Connection failed");
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
}

void loop() {
    sensor.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    temp = sensor.getTemperature();
    Serial.print(ax / (float)MAX_VALUE);
    Serial.print("\t");
    Serial.print(ay / (float)MAX_VALUE);
    Serial.print("\t");
    Serial.print(az / (float)MAX_VALUE);
    Serial.print("\t-\t");
    Serial.print(gx / (float)MAX_VALUE);
    Serial.print("\t");
    Serial.print(gy / (float)MAX_VALUE);
    Serial.print("\t");
    Serial.print(gz / (float)MAX_VALUE);
    Serial.print("\t-\t");
    Serial.println(temp / (float)MAX_VALUE);
    delay(50);
}
