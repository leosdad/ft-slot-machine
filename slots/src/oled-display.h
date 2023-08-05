
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef DEBUG_SLOTS_H
#define DEBUG_SLOTS_H

// -------------------------------------------------------------------- Includes

#include "../slots.h"
#include "oled-display-driver.h"
#include "game.h"

// ----------------------------------------------------------- Class declaration

class OledDisplay
{
	private:

		OledDisplayDriver odd;
		Payline payline;
		bool debugMode;
		uint16_t lastSize;

		void displayNumber(uint16_t number, uint8_t xPos, uint8_t prevDigits, uint8_t maxDigits);

	public:

		void Setup(bool debugMode);
		void DisplayBet(uint16_t number);
		void DisplayPayoff(uint16_t number, bool updateSize = false);
		void DisplayDebugInfo(Game game);
		void ShowState(const char *str);
};

// ------------------------------------------------------------------------- End

#endif // DEBUG_SLOTS_H
