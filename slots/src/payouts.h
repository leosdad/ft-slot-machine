
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#pragma once

// -------------------------------------------------------------------- Includes

#include "slots.h"
#include "game.h"

// ----------------------------------------------------------- Class declaration

class Payouts
{
	private:
		Payline payline;
		Awards highestAward;
		uint8_t payoutMultiplier = 1;

		uint16_t calculatePayline(Game game, int paylineIndex);

	public:
		bool SetMultiplier(uint8_t multiplier);
		Awards GetHighestAward(Game *game);
		uint16_t CalculateTotalPayout(Game *game);
};

// ------------------------------------------------------------------------- End
