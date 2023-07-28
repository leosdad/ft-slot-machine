
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

	public:

		void Setup(bool debugMode);
		void DisplayBigNumber(uint16_t number);

		// For debugging

		void ShowReelPreview(Game game);
		void ShowState(const char *str);
};

// ------------------------------------------------------------------------- End

#endif // DEBUG_SLOTS_H
