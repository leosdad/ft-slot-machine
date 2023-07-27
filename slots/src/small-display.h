
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef SMALL_DISPLAY_H
#define SMALL_DISPLAY_H

// -------------------------------------------------------------------- Includes

#include <Arduino.h>
#include "oled-display.h"

// ----------------------------------------------------------- Class declaration

class SmallDisplay
{
	private:
		OledDisplay od;
		bool displayDebugInfo;

	public:
		void Setup(bool displayDebugInfo);
		void DisplayBet(uint16_t nCoins);
};

// ------------------------------------------------------------------------- End

#endif // SMALL_DISPLAY_H
