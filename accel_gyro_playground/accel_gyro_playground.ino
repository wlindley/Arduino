#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "SampleBuffer.h"
#include "NewPing.h"
#include "Kalman.h"
#include "RGBLed.h"
#include "Led.h"
#include "Timer.h"
#include "DeltaTimer.h"

//#define PRINT_SAMPLES
#define PRINT_ROLL
#define PRINT_PITCH
#define PRINT_MOVEMENT
#define PRINT_AVERAGES
//#define PRINT_TEMP

const int DELAY = 20;
const int16_t FILTER_MASK = 0xFF80;
const float DEAD_ZONE = 200.f;
const float LAMP_TOGGLE_DELAY = 1.f;
const float LAMP_MOTION_THRESHOLD = 40000.f;

RGBLed led(11, 9, 10);
Led lamp(3, true);
bool lampState;
Timer lampTimer;
DeltaTimer deltaTimer;
MPU6050 sensor;
Kalman kalmanX, kalmanY;
FloatSampleBuffer bufferX, bufferY, bufferZ;
int16_t ax, ay, az, gx, gy, gz, temp;
double roll, pitch;
double gyroXAngle, gyroYAngle, kalXAngle, kalYAngle;

void setup() {
    led.off();
    lamp.on();
    lampState = false;
    lampTimer.setDelay(LAMP_TOGGLE_DELAY);
    lampTimer.update(LAMP_TOGGLE_DELAY);
    Serial.begin(9600);
    Serial.println("Initializing sensor");
    sensor.initialize();
    Serial.println(sensor.testConnection() ? "Connection successful" : "Connection failed");
    
    //initial readings
    sensor.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    roll = atan((float)ay / sqrt((float)ax * ax + (float)az * az)) * RAD_TO_DEG;
    pitch = atan2((float)-ax, (float)az) * RAD_TO_DEG;
    kalmanX.setAngle(roll);
    kalmanY.setAngle(pitch);
    gyroXAngle = kalXAngle = roll;
    gyroYAngle = kalYAngle = pitch;
    printReadings();
    Serial.println(" ");
    
    //calibration
    Serial.println("-----Calibration Starting-----");
    lamp.on();
    for (int i = 0; i < 500; i++) {
        updateSensor();
        if (i % 50 == 0) {
            printReadings();
        }
        delay(DELAY);
    }
    lamp.off();
    Serial.println("-----Calibration Complete-----");
    led.on();
    
    deltaTimer.updateDt();
}

void loop() {
    updateSensor();
    printReadings();
    led.setHSV((kalYAngle + 90.f) * 2.f, 1.f, 1.f);
    delay(DELAY);
}

void updateSensor() {
    float dt = deltaTimer.updateDt();
    lampTimer.update(dt);
    sensor.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
    ax &= FILTER_MASK;
    ay &= FILTER_MASK;
    az &= FILTER_MASK;
    
    #ifdef PRINT_SAMPLES
    Serial.print("x: ");
    Serial.print(ax);
    Serial.print(", y: ");
    Serial.print(ay);
    Serial.print(", z: ");
    Serial.print(az);
    Serial.print("\t");
    #endif
    
    //update accel
    bufferX.addSample((float)ax/* + 600.f*/);
    bufferY.addSample((float)ay/* - 450.f*/);
    bufferZ.addSample((float)az - 16500.f);
    checkLampActivation();
    
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

void checkLampActivation() {
    if (!lampTimer.isTriggered())
        return;
    float currentSample = (float)az - 17500.f;
    float currentAverage = bufferZ.getAverage();
    float diff = abs(currentSample - currentAverage);
    if (diff > LAMP_MOTION_THRESHOLD) {
        if (lampState) {
            lamp.off();
            lampTimer.reset();
            lampState = false;
        } else {
            lamp.on();
            lampTimer.reset();
            lampState = true;
        }
    }
}

void printReadings() {
    float xAccel = bufferX.getAverage();
    float yAccel = bufferY.getAverage();
    float zAccel = bufferZ.getAverage();
    
    #ifdef PRINT_ROLL
    Serial.print("roll: ");
    Serial.print(kalXAngle); Serial.print("\t");
    #endif
    
    #ifdef PRINT_PITCH
    Serial.print("pitch: ");
    Serial.print(kalYAngle); Serial.print("\t");
    #endif
    
    #ifdef PRINT_MOVEMENT
    Serial.print("movement: ");
    
    if (DEAD_ZONE < xAccel) {
        Serial.print("rght ");
    } else if (-DEAD_ZONE > xAccel) {
        Serial.print("left ");
    } else {
        Serial.print("---- ");
    }
    
    if (DEAD_ZONE < yAccel) {
        Serial.print("fwrd ");
    } else if (-DEAD_ZONE > yAccel) {
        Serial.print("bkwd ");
    } else {
        Serial.print("---- ");
    }
    
    if (DEAD_ZONE < zAccel) {
        Serial.print(" up ");
    } else if (-DEAD_ZONE > zAccel) {
        Serial.print("down");
    } else {
        Serial.print("----");
    }
    Serial.print("\t");
    #endif
    
    #ifdef PRINT_AVERAGES
    Serial.print("avg.x: ");
    Serial.print(xAccel);
    Serial.print(", avg.y: ");
    Serial.print(yAccel);
    Serial.print(", avg.z: ");
    Serial.print(zAccel);
    Serial.print("\t");
    #endif
    
    #ifdef PRINT_TEMP
    Serial.print("\ttemp: ");
    Serial.print(temp);
    #endif
    
    Serial.println(" ");
}
