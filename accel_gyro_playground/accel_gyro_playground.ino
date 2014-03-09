#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "SampleBuffer.h"
#include "NewPing.h"
#include "Kalman.h"

const int LED_PIN = 13;
const int NUM_SAMPLES = 1000;
const int DELAY = 20;

#define PRINT_AVERAGE

MPU6050 sensor;
Kalman kalmanX, kalmanY;
int16_t ax, ay, az, gx, gy, gz, temp;
double roll, pitch;
double gyroXAngle, gyroYAngle, compXAngle, compYAngle, kalXAngle, kalYAngle;
long prevTime;

void setup() {
    Serial.begin(9600);
    sensor.initialize();
    Serial.println(sensor.testConnection() ? "Connection successful" : "Connection failed");
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
    
    //initial readings
    sensor.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    roll = atan((float)ay / sqrt((float)ax * ax + (float)az * az)) * RAD_TO_DEG;
    pitch = atan2((float)-ax, (float)az) * RAD_TO_DEG;
    kalmanX.setAngle(roll);
    kalmanY.setAngle(pitch);
    gyroXAngle = compXAngle = kalXAngle = roll;
    gyroYAngle = compYAngle = kalYAngle = pitch;
    prevTime = millis();
    printReadings();
    Serial.println(" ");
    
    //calibration
    for (int i = 0; i < 500; i++) {
        updateSensor();
        if (i % 50 == 0) {
            printReadings();
        }
        delay(DELAY);
    }
    Serial.println(" ");
}

void loop() {
    updateSensor();
    double temp = sensor.getTemperature() / 340.0 + 36.53;
    printReadings();
    delay(DELAY);
}

void updateSensor() {
    sensor.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    long curTime = millis();
    float dt = curTime - prevTime / 1000.f;
    prevTime = curTime;
    
    roll = atan((float)ay / sqrt((float)ax * ax + (float)az * az)) * RAD_TO_DEG;
    pitch = atan2((float)-ax, (float)az) * RAD_TO_DEG;
    
    double gyroXRate = gx / 131.0;
    double gyroYRate = gy / 131.0;
    
    if ((pitch < -90.0 && kalYAngle > 90.0) || (pitch > 90.0 && kalYAngle < -90.f)) {
        kalmanY.setAngle(pitch);
        gyroYAngle = compYAngle = kalYAngle = pitch;
    } else {
        kalYAngle = kalmanY.getAngle(pitch, gyroYRate, dt);
    }
    
    if (abs(kalYAngle) > 90.0) {
        gyroXRate = -gyroXRate;
    }
    kalXAngle = kalmanX.getAngle(roll, gyroXRate, dt);
    
    gyroXAngle += gyroXRate * dt;
    gyroYAngle += gyroYRate * dt;
    
    compXAngle = .93 * (compXAngle + gyroXRate * dt) + .07 * roll;
    compYAngle = .93 * (compYAngle + gyroYRate * dt) + .07 * pitch;
    
    if (gyroXAngle < -180.0 || gyroXAngle > 180.0) {
        gyroXAngle = kalXAngle;
    }
    if (gyroYAngle < -180.0 || gyroYAngle > 180.0) {
        gyroYAngle = kalYAngle;
    }
}

void printReadings() {
    Serial.print("roll: ");
    Serial.print(roll); Serial.print("\t");
    Serial.print(gyroXAngle); Serial.print("\t");
    Serial.print(compXAngle); Serial.print("\t");
    Serial.print(kalXAngle); Serial.print("\t");
    Serial.print("\tpitch: ");
    Serial.print(pitch); Serial.print("\t");
    Serial.print(gyroYAngle); Serial.print("\t");
    Serial.print(compYAngle); Serial.print("\t");
    Serial.print(kalYAngle); Serial.print("\t");
    Serial.print("\ttemp: ");
    Serial.println(temp);
}
