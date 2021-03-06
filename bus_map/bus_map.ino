#include "Process.h"
#include "DeltaTimer.h"
#include "Timer.h"
#include "SevenSegmentDisplay.h"
#include "ShiftRegister.h"
#include <math.h>

const int PIN_DATA = 4;
const int PIN_CLOCK = 3;
const int PIN_LATCH = 2;

const float UPDATE_DELAY = 30.f; //in seconds
const float PROCESSING_DELAY = 5.f; //in seconds
const float INITIAL_DELAY = 30.f; //in seconds

struct ArrivalData {
    const char* stopId;
    const char* busId;
    float nextArrival;
};

//these are displayed in reverse order
enum ArrivalDataIds {
    ONE_TWENTY_EIGHT_NW,
    ONE_TWENTY_EIGHT_SE,
    TWENTY_ONE_EXPRESS,
    TWENTY_ONE,
    NUM_IDS,
};
ArrivalData arrivalData[4];
SevenSegmentDisplay* ssDisplay;
ShiftRegister* shifter;
byte data;

DeltaTimer deltaTimer;
Timer timer;

void setup() {
    Bridge.begin();
    Serial.begin(9600);
    
    ssDisplay = new SevenSegmentDisplay(&data, true);
    shifter = new ShiftRegister(PIN_DATA, PIN_CLOCK, PIN_LATCH);
    
    arrivalData[TWENTY_ONE].stopId = "22710";
    arrivalData[TWENTY_ONE].busId = "21"; //northbound
    
    arrivalData[TWENTY_ONE_EXPRESS].stopId = "22710";
    arrivalData[TWENTY_ONE_EXPRESS].busId = "21E"; //northbound
    
    arrivalData[ONE_TWENTY_EIGHT_NW].stopId = "40010";
    arrivalData[ONE_TWENTY_EIGHT_NW].busId = "128"; //north west bound
    
    arrivalData[ONE_TWENTY_EIGHT_SE].stopId = "36470";
    arrivalData[ONE_TWENTY_EIGHT_SE].busId = "128"; //south east bound
    
    for (int i = 0; i < NUM_IDS; i++) {
        arrivalData[i].nextArrival = NAN;
    }
    
    clearDisplays();
    
    delay(INITIAL_DELAY * 1000.f);
    
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
    for (int i = 0; i < NUM_IDS * 2; i++) {
        ssDisplay->displayDash();
        shifter->send(&data, 1);
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
        float nextArrival = getNextArrival(arrivalData[i].stopId, arrivalData[i].busId);
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
            ssDisplay->displayDash();
            shifter->send(&data, 1);
            ssDisplay->displayDash();
            shifter->send(&data, 1);
        } else {
            int minutes = round(arrivalTime / 60.f); //convert seconds to minutes
            Serial.println(minutes);
            ssDisplay->displayNumber(minutes % 10);
            shifter->send(&data, 1);
            ssDisplay->displayNumber(minutes / 10);
            shifter->send(&data, 1);
        }
    }
}

float getNextArrival(const char* stopId, const char* busId) {
    Process p;
    p.begin("/root/onebusaway/getNextArrivalAtStop.py");
    p.addParameter(stopId);
    p.addParameter(busId);
    p.run();
    char buffer[32];
    p.readString().toCharArray(buffer, 32);
    Serial.print("Result for stop id ");
    Serial.print(stopId);
    Serial.print(": ");
    Serial.println(buffer);
    return atof(buffer);
}
