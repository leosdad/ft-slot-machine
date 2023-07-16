
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef DEBUG_SLOTS_H
#define DEBUG_SLOTS_H

// -------------------------------------------------------------------- Includes

#include "../slots.h"

// ------------------------------------------------------------ Public functions

class Debug
{
	public:
		void Setup();
		void DisplayS(const char *msg, uint8_t row = 0, uint8_t col = 0, bool erase = false);
		void DisplayN(uint16_t number, uint8_t row = 0, uint8_t col = 0, bool erase = false);
		void ShowReelPreview(
			uint16_t totalSpins,
			uint16_t totalWins,
			uint8_t extraTurns[NREELS],
			uint16_t payoff[NREELS],
			uint16_t pos[NREELS]
		);

	private:
		uint8_t getLineSymbol(uint8_t line, uint8_t reel, uint16_t pos[]);
};

// ------------------------------------------------------------------------- End

#endif // DEBUG_SLOTS_H
