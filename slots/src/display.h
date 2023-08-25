
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
		void start();
		void show(const char *msg);
		void clear();
		void displayBet(uint8_t value);
};

// ------------------------------------------------------------------------- End
