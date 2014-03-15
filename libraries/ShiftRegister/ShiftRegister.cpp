#include "ShiftRegister.h"

ShiftRegister::ShiftRegister(int dataPin, int clockPin, int latchPin) : dataPin(dataPin), clockPin(clockPin), latchPin(latchPin)
{
	pinMode(dataPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(latchPin, OUTPUT);
}

void ShiftRegister::send(bool data[], unsigned int length)
{
	digitalWrite(latchPin, LOW);

	for (int i = 0; i < length; i++)
	{
		digitalWrite(clockPin, LOW);
		digitalWrite(dataPin, data[i] ? HIGH : LOW);
		digitalWrite(clockPin, HIGH);
	}

	digitalWrite(latchPin, HIGH);
}

void ShiftRegister::send(byte data[], unsigned int length)
{
	digitalWrite(latchPin, LOW);

	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			digitalWrite(clockPin, LOW);
			digitalWrite(dataPin, (data[i] & (1 << j)) ? HIGH : LOW);
			digitalWrite(clockPin, HIGH);
		}
	}

	digitalWrite(latchPin, HIGH);
}