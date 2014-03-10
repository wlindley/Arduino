#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "SampleBuffer.h"
#include "NewPing.h"
#include "Kalman.h"

const int LED_PIN = 13;
const int DELAY = 20;
const float Z_DEAD_ZONE = 200.f;

#define PRINT_AVERAGE

MPU6050 sensor;
Kalman kalmanX, kalmanY;
FloatSampleBuffer bufferZ;
int16_t ax, ay, az, gx, gy, gz, temp;
double roll, pitch;
double gyroXAngle, gyroYAngle, kalXAngle, kalYAngle;
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
    gyroXAngle = kalXAngle = roll;
    gyroYAngle = kalYAngle = pitch;
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
    printReadings();
    delay(DELAY);
}

void updateSensor() {
    sensor.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
    //update time
    long curTime = millis();
    float dt = curTime - prevTime / 1000.f;
    prevTime = curTime;
    
    //update z accel
    bufferZ.addSample((float)az - 17500.f);
    
    //update roll and pitch
    roll = atan((float)ay / sqrt((float)ax * ax + (float)az * az)) * RAD_TO_DEG;
    pitch = atan2((float)-ax, (float)az) * RAD_TO_DEG;
    
    //update Kalman filter
    double gyroXRate = gx / 131.0;
    double gyroYRate = gy / 131.0;
    
    if ((pitch < -90.0 && kalYAngle > 90.0) || (pitch > 90.0 && kalYAngle < -90.f)) {
        kalmanY.setAngle(pitch);
        gyroYAngle = kalYAngle = pitch;
    } else {
        kalYAngle = kalmanY.getAngle(pitch, gyroYRate, dt);
    }
    
    if (abs(kalYAngle) > 90.0) {
        gyroXRate = -gyroXRate;
    }
    kalXAngle = kalmanX.getAngle(roll, gyroXRate, dt);
    
    gyroXAngle += gyroXRate * dt;
    gyroYAngle += gyroYRate * dt;
    
    if (gyroXAngle < -180.0 || gyroXAngle > 180.0) {
        gyroXAngle = kalXAngle;
    }
    if (gyroYAngle < -180.0 || gyroYAngle > 180.0) {
        gyroYAngle = kalYAngle;
    }
    
    //update temperature
    temp = sensor.getTemperature() / 340.0 + 36.53;
}

void printReadings() {
    Serial.print("roll: ");
    Serial.print(kalXAngle); Serial.print("\t");
    Serial.print("\tpitch: ");
    Serial.print(kalYAngle); Serial.print("\t");
    Serial.print("\tz movement: ");
    float zAccel = bufferZ.getAverage();
    if (Z_DEAD_ZONE < zAccel) {
        Serial.print(" up ");
    } else if (-Z_DEAD_ZONE > zAccel) {
        Serial.print("down");
    } else {
        Serial.print("----");
    }
    Serial.print("\ttemp: ");
    Serial.println(temp);
}
