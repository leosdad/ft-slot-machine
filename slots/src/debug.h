
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef DEBUG_SLOTS_H
#define DEBUG_SLOTS_H

// -------------------------------------------------------------------- Includes

#include "../slots.h"
#include "oled-display.h"
#include "game.h"
#include "reel.h"
#include "payline.h"

// ----------------------------------------------------------- Class declaration

class Debug
{
	private:

		OledDisplay od;
		Payline payline;

	public:

		Debug();

		void Setup();
		void DisplayS(const char *msg, uint8_t row = 0, uint8_t col = 0, bool erase = false);
		void DisplayN(uint16_t number, uint8_t row = 0, uint8_t col = 0, bool erase = false);
		void ShowReelPreview(
			Game game,
			Reel reels[NREELS],
			uint16_t payoff[NPAYLINES]
		);
};

// ------------------------------------------------------------------------- End

#endif // DEBUG_SLOTS_H
