#include "Process.h"

void setup() {
    Bridge.begin();
    Serial.begin(9600);
}

void loop() {
    updateArrival("22710", "21");
    delay(5000);
    updateArrival("22710", "21E");
    delay(5000);
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
