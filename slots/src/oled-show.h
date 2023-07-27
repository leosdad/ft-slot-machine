
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
		bool displayDebugInfo;

	public:

		void Setup(bool displayDebugInfo);
		void DisplayBet(uint16_t nCoins);

		// Debug only

		void ShowReelPreview(Game game, Reel reels[NREELS], uint16_t payoff[NPAYLINES]);
		void ShowState(const char *str);
};

// ------------------------------------------------------------------------- End

#endif // DEBUG_SLOTS_H
