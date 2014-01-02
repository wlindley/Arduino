#include "MoodCueProgram.h"
#include <Arduino.h>

const int SENSOR_PIN = A0;

void MoodCueProgram::init()
{
	Program::init();

	pos = 0;
	speed = 90;
	servo.attach(9);
	pinMode(SENSOR_PIN, INPUT);
}

void MoodCueProgram::update(float dt)
{
	Program::update(dt);

	int value = analogRead(SENSOR_PIN);
	int out = map(value, 0, 1023, 0, 179);

	/*pos += dt * speed;
	if (pos >= 179 || pos <= 0)
	{
		speed *= -1;
		pos = min(max(pos, 0), 179);
	}*/
	
	Serial.print("input: ");
	Serial.print(value);
	Serial.print(", output: ");
	Serial.println(out);
	servo.write(out);
	delay(15);
}