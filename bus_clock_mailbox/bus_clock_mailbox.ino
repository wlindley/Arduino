#include <Bridge.h>
#include <Mailbox.h>
#include <Process.h>
#include <math.h>
#include "DeltaTimer.h"

const float MAX_DISPLAY_TIME = 50.f; //in minutes
const float UPDATE_DELAY = 20.f; //in seconds

struct ArrivalData {
    String stopId;
    String busId;
    String arrivalIndex;
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
    
    registerBuses();
    
    deltaTimer.updateDt();
}

void initializeBusStops() {
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
}

void registerBuses() {
    for (int i = 0; i < NUM_IDS; i++) {
        registerForUpdates(arrivalData[i].stopId, arrivalData[i].busId, arrivalData[i].arrivalIndex);
        requestImmediateUpdate(arrivalData[i].stopId, arrivalData[i].busId, arrivalData[i].arrivalIndex);
    }
}

void registerForUpdates(String stopId, String busId, String arrivalIndex) {
    Serial.println("--Registering for updates--");
    Serial.print("Stop id: ");
    Serial.println(stopId);
    Serial.print("Bus id: ");
    Serial.println(busId);
    Serial.print("Arrival index: ");
    Serial.println(arrivalIndex);
    
    Process p;
    p.begin("/root/onebusaway/registerForUpdates.py");
    p.addParameter(stopId);
    p.addParameter(busId);
    p.addParameter(arrivalIndex);
    p.run();
}

void requestImmediateUpdate(String stopId, String busId, String arrivalIndex) {
    Serial.println("--Requesting immediate update--");
    Serial.print("Stop id: ");
    Serial.println(stopId);
    Serial.print("Bus id: ");
    Serial.println(busId);
    Serial.print("Arrival index: ");
    Serial.println(arrivalIndex);
    
    Process p;
    p.begin("/root/onebusaway/updateMailboxWithArrivalInfo.py");
    p.addParameter(stopId);
    p.addParameter(busId);
    p.addParameter(arrivalIndex);
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
    for (int i = 0; i < NUM_IDS; i++) {
        if (!isnan(arrivalData[i].nextArrival)) {
            //Serial.print(" decrementing ");
            //Serial.print(arrivalData[i].nextArrival);
            //Serial.print(" to ");
            //Serial.println(arrivalData[i].nextArrival - dt);
            arrivalData[i].nextArrival -= dt;
            if (0 > arrivalData[i].nextArrival) {
                arrivalData[i].nextArrival = NAN;
                requestImmediateUpdate(arrivalData[i].stopId, arrivalData[i].busId, arrivalData[i].arrivalIndex);
            }
        }
    }
}

void checkMailbox() {
    while (Mailbox.messageAvailable()) {
        String message;
        Mailbox.readMessage(message);
        message.trim();
        
        String stopId = getStopId(message);
        String busId = getBusId(message);
        String arrivalIndex = getArrivalIndex(message);
        float arrivalDelta = getArrivalDelta(message);
        
        Serial.println("--Message received--");
        Serial.print("Stop Id: ");
        Serial.println(stopId);
        Serial.print("Bus Id: ");
        Serial.println(busId);
        Serial.print("Arrival Index: ");
        Serial.println(arrivalIndex);
        Serial.print("Arrival Delta: ");
        Serial.println(arrivalDelta);
        
        for (int i = 0; i < NUM_IDS; i++) {
            if (0 == arrivalData[i].stopId.compareTo(stopId)
                && 0 == arrivalData[i].busId.compareTo(busId)
                && 0 == arrivalData[i].arrivalIndex.compareTo(arrivalIndex)) {
                arrivalData[i].nextArrival = arrivalDelta;
            }
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

String getStopId(String message) {
    return getCSVWithIndex(message, 0);
}

String getBusId(String message) {
    return getCSVWithIndex(message, 1);
}

String getArrivalIndex(String message) {
    return getCSVWithIndex(message, 2);
}

float getArrivalDelta(String message) {
    String stringDelta = getCSVWithIndex(message, 3);
    if (0 >= stringDelta.length()) {
        return NAN;
    }
    char buff[32];
    stringDelta.toCharArray(buff, sizeof(buff));
    return atof(buff);
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
            int minutes = round(arrivalTime / 60.f); //convert seconds to minutes
            //Serial.println(minutes);
            analogWrite(arrivalData[i].pinId, constrain((minutes / MAX_DISPLAY_TIME) * 255.f, 0, 255)); //convert minutes to 0-255
        }
    }
}
