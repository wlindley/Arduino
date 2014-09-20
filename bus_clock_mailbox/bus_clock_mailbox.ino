#include <Bridge.h>
#include <Mailbox.h>
#include <Process.h>
#include <math.h>
#include "DeltaTimer.h"

const float MAX_DISPLAY_TIME = 20.f; //in minutes
const float UPDATE_DELAY = 20.f; //in seconds

struct ArrivalData {
    float nextArrival;
    int pinId;
};

const int NUM_IDS = 3;
ArrivalData arrivalData[3];

DeltaTimer deltaTimer;

void setup() {
    Serial.begin(9600);
    
    initializeBusStops();
    clearDisplays();
    
    Bridge.begin();
    Mailbox.begin();
    
    requestImmediateUpdate();
    
    deltaTimer.updateDt();
}

void initializeBusStops() {
    for (int i = 0; i < NUM_IDS; i++) {
        arrivalData[i].nextArrival = NAN;
        pinMode(arrivalData[i].pinId, OUTPUT);
    }
}

void requestImmediateUpdate() {
    Serial.println("--Requesting immediate update--");
    
    Process p;
    p.begin("/root/onebusaway/scheduledUpdater.py");
    p.run();
}

void clearDisplays() {
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < NUM_IDS; i++) {
            analogWrite(arrivalData[i].pinId, 0);
        }
        delay(500);
        for (int i = 0; i < NUM_IDS; i++) {
            analogWrite(arrivalData[i].pinId, 255);
        }
        delay(500);
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
    //Serial.println("decrementing timers");
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
    for (int i = 0; i < NUM_IDS; i++) {
        //Serial.print(arrivalData[i].busId);
        //Serial.print(": ");
        float arrivalTime = arrivalData[i].nextArrival;
        if (isnan(arrivalTime) || 0 > arrivalTime) {
            //Serial.println("--");
            analogWrite(arrivalData[i].pinId, 255);
        } else {
            float minutes = arrivalTime / 60.f; //convert seconds to minutes
            //Serial.println(minutes);
            analogWrite(arrivalData[i].pinId, constrain((minutes / MAX_DISPLAY_TIME) * 255.f, 0, 255)); //convert minutes to 0-255
        }
    }
}
