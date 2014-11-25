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

#define PRINT_ROLL
#define PRINT_PITCH
#define PRINT_MOVEMENT

enum GestureState {
    GS_UNKNOWN,
    GS_START,
    GS_MOVED_RIGHT,
    GS_MOVED_DOWN,
    GS_MOVED_LEFT,
    GS_MOVED_UP
} gestureState = GS_UNKNOWN;

const int DELAY = 20;
const float DEAD_ZONE = 200.f;
const float STATE_ACTIVATION_TIME = 1.f;

Led lamp(3, true);
bool lampState;

DeltaTimer deltaTimer;
Timer stateTimer;
float xMovementAccumulator, yMovementAccumulator, zMovementAccumulator;

MPU6050 sensor;
Kalman kalmanX, kalmanY;
FloatSampleBuffer bufferX, bufferY, bufferZ;
int16_t ax, ay, az, gx, gy, gz, temp;
double roll, pitch;
double gyroXAngle, gyroYAngle, kalXAngle, kalYAngle;

void setup() {
    lamp.off();
    lampState = false;
    stateTimer.setDelay(STATE_ACTIVATION_TIME);
    Serial.begin(9600);
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
    xMovementAccumulator = yMovementAccumulator = zMovementAccumulator = 0.f;
    printReadings();
    Serial.println(" ");
    
    //calibration
    Serial.println("-----Calibration Starting-----");
    lamp.on();
    for (int i = 0; i < 500; i++) {
        updateSensor(DELAY);
        if (i % 50 == 0) {
            printReadings();
        }
        delay(DELAY);
    }
    lamp.off();
    Serial.println("-----Calibration Complete-----");
    
    deltaTimer.updateDt();
}

void loop() {
    float dt = deltaTimer.updateDt();
    updateSensor(dt);
    checkLampActivation(dt);
    printReadings();
    delay(DELAY);
}

void updateSensor(float dt) {
    sensor.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
    //update accel
    bufferX.addSample(getXReading());
    bufferY.addSample(getYReading());
    bufferZ.addSample(getZReading());
    
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
}

float getXReading() {
    return (float)ax + 600.f;
}

float getYReading() {
    return (float)ay - 450.f;
}

float getZReading() {
    return (float)az - 17500.f;
}

void checkLampActivation(float dt) {
    if (GS_UNKNOWN == gestureState) {
        if (5.f <= abs(kalXAngle) && 5.f <= abs(kalYAngle)) {
            stateTimer.update(dt);
            if (stateTimer.isTriggered()) {
                gestureState = GS_START;
                stateTimer.reset();
                xMovementAccumulator = yMovementAccumulator = zMovementAccumulator = 0.f;
            }
        } else {
            stateTimer.reset();
        }
    } else if (GS_START == gestureState) {
        xMovementAccumulator += bufferX.getAverage();
        yMovementAccumulator += bufferY.getAverage();
        zMovementAccumulator += bufferZ.getAverage();
        
        if (5000 <= abs(yMovementAccumulator) || 5000 <= abs(zMovementAccumulator)) {
            gestureState = GS_UNKNOWN;
        } else if (10000 <= xMovementAccumulator) {
            gestureState = GS_MOVED_RIGHT;
            xMovementAccumulator = yMovementAccumulator = zMovementAccumulator = 0.f;
            lamp.on();
        } else if (-10000 >= xMovementAccumulator) {
            gestureState = GS_MOVED_LEFT;
            xMovementAccumulator = yMovementAccumulator = zMovementAccumulator = 0.f;
        }
    }
    
    
    
    
    
    
    
    
    
    
    
    /*if (!lampTimer.isTriggered())
        return;
    float currentSample = getZReading();
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
    }*/
}

void printReadings() {
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
    float xAccel = bufferX.getAverage();
    float yAccel = bufferY.getAverage();
    float zAccel = bufferZ.getAverage();
    
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
    
    Serial.println(" ");
}
