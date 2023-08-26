
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#pragma once

// -------------------------------------------------------------------- Includes

#include "slots.h"
#include "game.h"

// --------------------------------------------------------------------- Defines

#define spJackpot SpecialFeatures::JACKPOT
#define spNone SpecialFeatures::NONE

// ----------------------------------------------------------- Class declaration

class Payoffs
{
	private:
		static Payline payline;

		static uint16_t calculatePayline(Game game, int paylineIndex);

	public:
		static uint16_t CalculateTotalPayoff(Game *game);
};

// ------------------------------------------------------------------------- End
