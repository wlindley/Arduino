#include "Process.h"
#include "DeltaTimer.h"
#include "Timer.h"
#include "SevenSegmentDisplay.h"
#include "ShiftRegister.h"

const float UPDATE_DELAY = 5.f; //in seconds
const float PROCESSING_DELAY = 1.f; //in seconds

struct ArrivalData {
    const char* stopId;
    const char* busId;
    float nextArrival;
};

ArrivalData arrivalData[4];
enum ArrivalDataIds {
    TWENTY_ONE,
    TWENTY_ONE_EXPRESS,
    ONE_TWENTY_EIGHT,
    ONE_TWENTY,
    NUM_IDS,
};

DeltaTimer deltaTimer;
Timer timer;

void setup() {
    Bridge.begin();
    Serial.begin(9600);
    
    arrivalData[TWENTY_ONE].stopId = "22710";
    arrivalData[TWENTY_ONE].busId = "21";
    
    arrivalData[TWENTY_ONE_EXPRESS].stopId = "22710";
    arrivalData[TWENTY_ONE_EXPRESS].busId = "21E";
    
    arrivalData[ONE_TWENTY_EIGHT].stopId = "40010";
    arrivalData[ONE_TWENTY_EIGHT].busId = "128";
    
    arrivalData[ONE_TWENTY].stopId = "21500";
    arrivalData[ONE_TWENTY].busId = "120";
    
    for (int i = 0; i < NUM_IDS; i++) {
        arrivalData[i].nextArrival = -1;
    }
    
    timer.setDelay(UPDATE_DELAY);
    
    deltaTimer.updateDt();
}

void loop() {
    float dt = deltaTimer.updateDt();
    timer.update(dt);
    
    decrementTimers(dt);
    
    if (timer.isTriggered()) {
        updateArrivals();
        timer.reset();
    }
    
    displayTimes();
    
    delay(PROCESSING_DELAY * 1000.f);
}

void decrementTimers(float dt) {
    for (int i = 0; i < NUM_IDS; i++) {
        arrivalData[i].nextArrival -= dt;
    }
}

void updateArrivals() {
    for (int i = 0; i < NUM_IDS; i++) {
        arrivalData[i].nextArrival = getNextArrival(arrivalData[i].stopId, arrivalData[i].busId);
    }
}

void displayTimes() {
    for (int i = 0; i < NUM_IDS; i++) {
        printResult(arrivalData[i].busId, round(arrivalData[i].nextArrival));
    }
}

void updateArrival(const char* stopId, const char* busId) {
    printResult(busId, getNextArrival(stopId, busId));
}

int getNextArrival(const char* stopId, const char* busId) {
    Process p;
    p.begin("/root/onebusaway/getNextArrivalAtStop.py");
    p.addParameter(stopId);
    p.addParameter(busId);
    p.run();
    return p.exitValue();
}

void printResult(const char* busId, int minutes) {
    Serial.print(busId);
    Serial.print(": ");
    if (0 > minutes || 100 <= minutes) {
        Serial.println("--");
    } else {
        Serial.println(minutes);
    }
}
