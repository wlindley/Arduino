const int NUM_PINS = 3;
const int PINS[] = {3, 10, 11};
const int SLEEP_TIME = 50;

float values[] = {0, 127, 255};
float deltas[] = {2, 3, -4};

void setup() {
    for (int i = 0; i < NUM_PINS; i++) {
        pinMode(PINS[i], OUTPUT);
        analogWrite(PINS[i], 0);
    }
    delay(500);
    for (int i = 0; i < NUM_PINS; i++) {
        analogWrite(PINS[i], 255);
    }
    delay(500);
    for (int i = 0; i < NUM_PINS; i++) {
        analogWrite(PINS[i], 0);
    }
    delay(500);
}

void loop() {
    for (int i = 0; i < NUM_PINS; i++) {
        analogWrite(PINS[i], constrain(values[i], 0, 255));
        values[i] += deltas[i];
        if (values[i] >= 255 || values[i] <= 0) {
            deltas[i] *= -1;
        }
    }
    delay(SLEEP_TIME);
}
