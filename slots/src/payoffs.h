
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#pragma once

// -------------------------------------------------------------------- Includes

#include "slots.h"
#include "game.h"

// ----------------------------------------------------------- Class declaration

class Payoffs
{
	private:
		Payline payline;
		SpecialFeatures highestFeature;

		uint16_t calculatePayline(Game game, int paylineIndex);

	public:
		SpecialFeatures GetHighestFeature();
		uint16_t CalculateTotalPayoff(Game *game);
};

// ------------------------------------------------------------------------- End
