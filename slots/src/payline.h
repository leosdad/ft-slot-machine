
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#pragma once

// -------------------------------------------------------------------- Includes

#include "slots.h"
#include "reel.h"

// ----------------------------------------------------------- Class declaration

class Payline
{
	public:
		uint16_t Payoff = 0;

		uint8_t GetLineSymbol(uint8_t line, uint8_t nReel, Reel reel);
};

// ------------------------------------------------------------------------- End
