
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef DEBUG_SLOTS_H
#define DEBUG_SLOTS_H

// -------------------------------------------------------------------- Includes

#include "../slots.h"
#include "oled-display-driver.h"
#include "game.h"
#include "reel.h"
#include "payline.h"

// ----------------------------------------------------------- Class declaration

class OledShow
{
	private:

		OledDisplayDriver odd;
		Payline payline;
		bool debugMode;

		uint8_t getSize(uint8_t number);

	public:

		void Setup(bool debugMode);
		uint16_t DisplayBet(uint16_t number);
		uint16_t DisplayCoins(uint16_t number);
		void DisplayDebugInfo(Game game);
		void ShowState(const char *str);
};

// ------------------------------------------------------------------------- End

#endif // DEBUG_SLOTS_H
