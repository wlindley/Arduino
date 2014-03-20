#include "SevenSegmentDisplay.h"
#include "ShiftRegister.h"

byte data;
SevenSegmentDisplay* ssDisplay;
ShiftRegister* shift;
int counter = 0;

void setup() {
    Serial.begin(9600);
    ssDisplay = new SevenSegmentDisplay(&data, true);
    shift = new ShiftRegister(4, 2, 3);
    ssDisplay->displayDash();
    shift->send(&data, 1);
    delay(5000);
}

void loop() {
    ssDisplay->displayNumber(counter);
    shift->send(&data, 1);
    delay(2000);
    counter++;
    if (counter >= 10) {
        counter = 0;
    }
}
