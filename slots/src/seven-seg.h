
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef SEVEN_SEG_H
#define SEVEN_SEG_H

// -------------------------------------------------------------------- Includes

#include "../slots.h"
#include <Wire.h>
#include <SevenSegDisplay.h>

// ----------------------------------------------------------- Class declaration

class SevenSeg
{
	private:
		char displayBuffer[DISPLAYCHARS];

	public:
		void Setup();
		void DisplayNumber(long value, int lastPos = DISPLAYCHARS - 1);
		void ScrollMessage(char* msg);
		void FlashMessage(char* msg);
};

// ------------------------------------------------------------------------- End

#endif // SEVEN_SEG_H
