const int ONBOARD_LED = 13;
const int RED_LEDS[2] = {4, 5};
const int GREEN_LED = 3;
const int BUTTON = 2;
const int DELAY_TIME = 500;

int buttonState = 0;
int visibleRed = 0;
unsigned long prevTime = 0;
long timer = DELAY_TIME;

void setGreenState(int state) {
  digitalWrite(GREEN_LED, state);
}

void setRedState(int firstState, int secondState) {
  digitalWrite(RED_LEDS[0], firstState);
  digitalWrite(RED_LEDS[1], secondState);
}

void displayRedState() {
  int firstState = 0 == visibleRed ? LOW : HIGH;
  int secondState = 0 == visibleRed ? HIGH : LOW;
  setRedState(firstState, secondState);
}

void setup() {
  Serial.begin(9600);
  
  pinMode(RED_LEDS[0], OUTPUT);
  pinMode(RED_LEDS[1], OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUTTON, INPUT);
  
  pinMode(ONBOARD_LED, OUTPUT);
  digitalWrite(ONBOARD_LED, LOW);
  
  prevTime = millis();
}

void loop() {
  unsigned long curTime = millis();
  buttonState = digitalRead(BUTTON);
  if (LOW == buttonState) {
    setGreenState(HIGH);
    setRedState(LOW, LOW);
  } 
  else {
    setGreenState(LOW);
    displayRedState();

    if (timer > 0) {
      timer -= curTime - prevTime;
    } 
    else {
      visibleRed = 0 == visibleRed ? 1 : 0;
      timer = DELAY_TIME;
    }
  }
  prevTime = curTime;
}



