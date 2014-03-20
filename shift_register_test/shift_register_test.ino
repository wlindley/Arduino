#include "SevenSegmentDisplay.h"
#include "ShiftRegister.h"

byte data;
SevenSegmentDisplay* ssDisplay;
ShiftRegister* shift;

void setup() {
    Serial.begin(9600);
    data = 1;
    ssDisplay = new SevenSegmentDisplay(&data);
    shift = new ShiftRegister(4, 2, 3);
    shift->send(&data, 1);
}

void loop() {
    data = data << 1;
    Serial.println(data, BIN);
    shift->send(&data, 1);
    delay(2000);
}
