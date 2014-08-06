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
    String stopId;
    String busId;
    String arrivalIndex;
    float nextArrival;
};

ArrivalData arrivalData[4];
//these are displayed in reverse order
enum ArrivalDataIds {
    ONE_TWENTY_EIGHT_NW,
    ONE_TWENTY_EIGHT_SE,
    TWENTY_ONE_EXPRESS,
    TWENTY_ONE,
    NUM_IDS,
};

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
    arrivalData[TWENTY_ONE].stopId = "22710";
    arrivalData[TWENTY_ONE].busId = "21"; //northbound
    arrivalData[TWENTY_ONE].arrivalIndex = "0";
    
    arrivalData[TWENTY_ONE_EXPRESS].stopId = "22710";
    arrivalData[TWENTY_ONE_EXPRESS].busId = "21E"; //northbound
    arrivalData[TWENTY_ONE_EXPRESS].arrivalIndex = "0";
    
    arrivalData[ONE_TWENTY_EIGHT_NW].stopId = "40010";
    arrivalData[ONE_TWENTY_EIGHT_NW].busId = "128"; //north west bound
    arrivalData[ONE_TWENTY_EIGHT_NW].arrivalIndex = "0";
    
    arrivalData[ONE_TWENTY_EIGHT_SE].stopId = "36470";
    arrivalData[ONE_TWENTY_EIGHT_SE].busId = "128"; //south east bound
    arrivalData[ONE_TWENTY_EIGHT_SE].arrivalIndex = "0";
    
    for (int i = 0; i < NUM_IDS; i++) {
        arrivalData[i].nextArrival = NAN;
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
    Serial.println("--Decrementing timers--");
    for (int i = 0; i < NUM_IDS; i++) {
        if (!isnan(arrivalData[i].nextArrival)) {
            Serial.print(" decrementing ");
            Serial.print(arrivalData[i].nextArrival);
            Serial.print(" to ");
            Serial.println(arrivalData[i].nextArrival - dt);
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
    Serial.println("--Displaying times--");
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
