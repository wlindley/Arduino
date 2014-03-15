#ifndef __SEVENSEGMENTDISPLAY_H__
#define __SEVENSEGMENTDISPLAY_H__

#include <Arduino.h>

class SevenSegmentDisplay
{
public:
	SevenSegmentDisplay(byte* data);
	void displayNumber(byte number, bool decimalPoint=false);
	void displayCustom(byte values);
	void displayCustom(bool values[], unsigned int numValues);

private:
	byte* data;
	static const byte PATTERNS[];
};

#endif