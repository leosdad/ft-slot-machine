
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef PAYOFFS_H
#define PAYOFFS_H

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

#endif // PAYOFFS_H
