
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
		void show(const char *msg);
		void showFull(const char *msg);
		void show(uint8_t value);
		void showBet(uint8_t value);
};

// ------------------------------------------------------------------------- End
