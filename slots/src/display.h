
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
		void loop();
		void clear();
		void clearAll();
		void show(const char *msg);
		void showAt(const char *msg, uint8_t pos, bool clear = false);
		void showAll(const char *msg, uint8_t pos = 0);
		void showCentered(const char* msg);
		void blink(bool blink, uint32_t interval = 300);
		void showNumber(uint16_t value, bool clear);
		void showBet(uint16_t value);
		void showLife(uint16_t value);
		void scroll(const char *msg);
		void scrollAll(const char *msg);
};

// ------------------------------------------------------------------------- End
