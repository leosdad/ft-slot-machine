
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#pragma once

// -------------------------------------------------------------------- Includes

#include "drivers/led-matrix.h"

// ------------------------------------------------------------------- Variables

extern LedMatrix ledMatrix;

// ----------------------------------------------------------- Class declaration

class Display
{
	public:
		void setup();
		void clear();
		void show(uint8_t value, bool clear);
		void showBet(uint8_t value);
		void scroll(const char *msg);
		void scrollAll(const char *msg);
};

// ------------------------------------------------------------------------- End
