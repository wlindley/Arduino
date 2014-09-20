#include <Bridge.h>
#include <Mailbox.h>
#include <Process.h>
#include <math.h>
#include "DeltaTimer.h"
#include "SevenSegmentDisplay.h"
#include "ShiftRegister.h"

const int PIN_DATA = 4;
const int PIN_CLOCK = 3;
const int PIN_LATCH = 2;

const float MAX_DISPLAY_TIME = 50.f; //in minutes
const float UPDATE_DELAY = 20.f; //in seconds

struct ArrivalData {
    float nextArrival;
};

const int NUM_IDS = 4;
ArrivalData arrivalData[4]; //these are displayed in reverse order

DeltaTimer deltaTimer;
SevenSegmentDisplay* ssDisplay;
ShiftRegister* shifter;
byte data;

void setup() {
    Serial.begin(9600);
    
    initializeDisplay();
    clearDisplays();
    
    Bridge.begin();
    Mailbox.begin();
    
    initializeBusStops();
    
    deltaTimer.updateDt();
}

void initializeDisplay() {
    ssDisplay = new SevenSegmentDisplay(&data, true);
    shifter = new ShiftRegister(PIN_DATA, PIN_CLOCK, PIN_LATCH);
}

void initializeBusStops() {
    for (int i = 0; i < NUM_IDS; i++) {
        arrivalData[i].nextArrival = NAN;
    }
    requestImmediateUpdate();
}

void requestImmediateUpdate() {
    Serial.println("--Requesting immediate update--");
    
    Process p;
    p.begin("/root/onebusaway/scheduledUpdater.py");
    p.run();
}

void clearDisplays() {
    for (int i = 0; i < NUM_IDS * 2; i++) {
        ssDisplay->displayDash();
        shifter->send(&data, 1);
    }
}

void loop() {
    float dt = deltaTimer.updateDt();
    decrementTimers(dt);
    checkMailbox();
    displayTimes();
    delay(UPDATE_DELAY * 1000.f);
}

void decrementTimers(float dt) {
    //Serial.println("--Decrementing timers--");
    bool needsUpdate = false;
    for (int i = 0; i < NUM_IDS; i++) {
        if (!isnan(arrivalData[i].nextArrival)) {
            //Serial.print(" decrementing ");
            //Serial.print(arrivalData[i].nextArrival);
            //Serial.print(" to ");
            //Serial.println(arrivalData[i].nextArrival - dt);
            arrivalData[i].nextArrival -= dt;
            if (0 > arrivalData[i].nextArrival) {
                arrivalData[i].nextArrival = NAN;
                needsUpdate = true;
            }
        }
    }
    
    if (needsUpdate) {
        requestImmediateUpdate();
    }
}

void checkMailbox() {
    while (Mailbox.messageAvailable()) {
        String message;
        Mailbox.readMessage(message);
        message.trim();
        
        int displayIndex = getDisplayIndex(message);
        float arrivalDelta = getArrivalDelta(message);
        
        Serial.println("--Message received--");
        Serial.print("Display Index: ");
        Serial.println(displayIndex);
        Serial.print("Arrival Delta: ");
        Serial.println(arrivalDelta);
        
        if (0 <= displayIndex && NUM_IDS > displayIndex) {
            arrivalData[displayIndex].nextArrival = arrivalDelta;
        }
    }
}

String getCSVWithIndex(String message, int index) {
    int prevComma = -1;
    int curComma = -1;
    for (int i = 0; i < index + 1; i++) {
        prevComma = curComma;
        curComma = message.indexOf(",", curComma + 1);
        if (-1 == curComma) {
            if (-1 == prevComma) {
                return "";
            } else {
                curComma = message.length();
            }
        }
    }
    return message.substring(prevComma + 1, curComma);
}

float convertStringToFloat(String number) {
    if (0 >= number.length()) {
        return NAN;
    }
    char buff[32];
    number.toCharArray(buff, sizeof(buff));
    return atof(buff);
}

int getDisplayIndex(String message) {
    String displayIndex = getCSVWithIndex(message, 0);
    return displayIndex.toInt();
}

float getArrivalDelta(String message) {
    String stringDelta = getCSVWithIndex(message, 1);
    return convertStringToFloat(stringDelta);
}

void displayTimes() {
    Serial.println("--Displaying times--");
    for (int i = 0; i < NUM_IDS; i++) {
        Serial.print(i);
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
