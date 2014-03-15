#include "Process.h"

void setup() {
    Serial.begin(9600);
}

void loop() {
    Process p;
    p.begin("/root/onebusaway/getNextArrivalAtStop.py");
    p.addParameter("22710");
    p.addParameter("21");
    p.run();
    Serial.println(p.exitValue());
    
    delay(5000);
}
