#include "Process.h"
#include "DeltaTimer.h"
#include "Timer.h"
#include <math.h>

const float UPDATE_DELAY = 30.f; //in seconds
const float PROCESSING_DELAY = 5.f; //in seconds
const float INITIAL_DELAY = 10.f; //in seconds
const float MAX_DISPLAY_TIME = 50.f; //in minutes

struct ArrivalData {
    const char* stopId;
    const char* busId;
    const char* arrivalIndex;
    float nextArrival;
    int pinId;
};

const int NUM_IDS = 3;
ArrivalData arrivalData[4];

DeltaTimer deltaTimer;
Timer timer;

void setup() {
    Bridge.begin();
    Serial.begin(9600);
    
    arrivalData[0].stopId = "22710";
    arrivalData[0].busId = "21"; //northbound
    arrivalData[0].arrivalIndex = "0";
    arrivalData[0].pinId = 3;
    
    arrivalData[1].stopId = "22710";
    arrivalData[1].busId = "21"; //northbound
    arrivalData[1].arrivalIndex = "1";
    arrivalData[1].pinId = 10;
    
    arrivalData[2].stopId = "22710";
    arrivalData[2].busId = "21"; //northbound
    arrivalData[2].arrivalIndex = "2";
    arrivalData[2].pinId = 11;
    
    for (int i = 0; i < NUM_IDS; i++) {
        arrivalData[i].nextArrival = NAN;
        pinMode(arrivalData[i].pinId, OUTPUT);
    }
    
    clearDisplays();
    
    //delay(INITIAL_DELAY * 1000.f);
    
    timer.setDelay(UPDATE_DELAY);
    timer.update(UPDATE_DELAY); //set timer to zero so that it updates timers immediately
    
    deltaTimer.updateDt();
}

void loop() {
    float dt = deltaTimer.updateDt();
    timer.update(dt);
    
    decrementTimers(dt);
    
    if (timer.isTriggered() || busHasArrived()) {
        updateArrivals();
        timer.reset();
    }
    
    displayTimes();
    Serial.println("----------------------");
    
    delay(PROCESSING_DELAY * 1000.f);
}

void clearDisplays() {
    for (int i = 0; i < NUM_IDS; i++) {
        analogWrite(arrivalData[i].pinId, 0);
    }
    delay(500);
    for (int i = 0; i < NUM_IDS; i++) {
        analogWrite(arrivalData[i].pinId, 255);
    }
    delay(500);
    for (int i = 0; i < NUM_IDS; i++) {
        analogWrite(arrivalData[i].pinId, 0);
    }
    delay(500);
    for (int i = 0; i < NUM_IDS; i++) {
        analogWrite(arrivalData[i].pinId, 255);
    }
    delay(500);
    for (int i = 0; i < NUM_IDS; i++) {
        analogWrite(arrivalData[i].pinId, 0);
    }
}

void decrementTimers(float dt) {
    Serial.println("decrementing timers");
    for (int i = 0; i < NUM_IDS; i++) {
        if (!isnan(arrivalData[i].nextArrival)) {
            Serial.print(" decrementing ");
            Serial.print(arrivalData[i].nextArrival);
            Serial.print(" to ");
            Serial.println(arrivalData[i].nextArrival - dt);
            arrivalData[i].nextArrival -= dt;
            if (0 > arrivalData[i].nextArrival) {
                arrivalData[i].nextArrival = atof("nan");
            }
        }
    }
}

bool busHasArrived() {
    for (int i = 0; i < NUM_IDS; i++) {
        if (!isnan(arrivalData[i].nextArrival) && 0 >= arrivalData[i].nextArrival) {
            return true;
        }
    }
    return false;
}

void updateArrivals() {
    Serial.println("updating arrivals");
    for (int i = 0; i < NUM_IDS; i++) {
        float nextArrival = getNextArrival(arrivalData[i].stopId, arrivalData[i].busId, arrivalData[i].arrivalIndex);
        if (!isnan(nextArrival)) {
            arrivalData[i].nextArrival = nextArrival;
        }
    }
}

void displayTimes() {
    for (int i = 0; i < NUM_IDS; i++) {
        Serial.print(arrivalData[i].busId);
        Serial.print(": ");
        float arrivalTime = arrivalData[i].nextArrival;
        if (isnan(arrivalTime) || 0 > arrivalTime) {
            Serial.println("--");
            analogWrite(arrivalData[i].pinId, 255);
        } else {
            int minutes = round(arrivalTime / 60.f); //convert seconds to minutes
            Serial.println(minutes);
            analogWrite(arrivalData[i].pinId, constrain((minutes / MAX_DISPLAY_TIME) * 255.f, 0, 255)); //convert minutes to 0-255
        }
    }
}

float getNextArrival(const char* stopId, const char* busId, const char* arrivalIndex) {
    Process p;
    p.begin("/root/onebusaway/getNextArrivalAtStop.py");
    p.addParameter(stopId);
    p.addParameter(busId);
    p.addParameter(arrivalIndex);
    p.run();
    char buffer[32];
    p.readString().toCharArray(buffer, 32);
    Serial.print("Result for stop id ");
    Serial.print(stopId);
    Serial.print(": ");
    Serial.println(buffer);
    return atof(buffer);
}
