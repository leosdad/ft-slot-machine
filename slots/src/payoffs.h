
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef PAYOFFS_H
#define PAYOFFS_H

// -------------------------------------------------------------------- Includes

#include "../slots.h"
#include "game.h"

// ---------------------------------------------------------------------- Struct

enum class SpecialFeatures {
	NONE = 0x00,
	BONUS = 0x01,
	JACKPOT = 0x02,
};

/**
 * Defines winning combinations, payoff values and special features.
 */
struct payoffItem {
	uint8_t symbol[NREELS];
	uint16_t payoff;
	SpecialFeatures features;
};

// ------------------------------------------------------------------- Constants

// Global static constants

static const uint8_t payoffMultiplier = PAYMULTIPLIER;

/**
 * 0 (zero) here means any symbol.
 */
static constexpr payoffItem payoffTable[NCOMBINATIONS] = {
	{{1, 1, 1}, 172, SpecialFeatures::JACKPOT},
	{{3, 3, 3}, 86, SpecialFeatures::JACKPOT},
	{{4, 4, 4}, 86, SpecialFeatures::JACKPOT},
	{{5, 5, 3}, 58, SpecialFeatures::NONE},
	{{8, 8, 3}, 43, SpecialFeatures::NONE},
	{{2, 2, 3}, 29, SpecialFeatures::NONE},
	{{8, 8, 8}, 22, SpecialFeatures::JACKPOT},
	{{5, 5, 5}, 20, SpecialFeatures::NONE},
	{{2, 2, 2}, 14, SpecialFeatures::NONE},
	{{1, 1, 0}, 14, SpecialFeatures::NONE},
	{{0, 1, 1}, 14, SpecialFeatures::NONE},
	{{1, 0, 1}, 14, SpecialFeatures::NONE},
	{{0, 3, 3}, 14, SpecialFeatures::NONE},
	{{0, 3, 1}, 14, SpecialFeatures::NONE},
	{{0, 4, 4}, 14, SpecialFeatures::NONE},
	{{5, 3, 0}, 14, SpecialFeatures::NONE},
	{{6, 6, 0}, 7, SpecialFeatures::NONE},
	{{3, 3, 0}, 7, SpecialFeatures::NONE},
	{{3, 1, 0}, 7, SpecialFeatures::NONE},
	{{8, 8, 0}, 4, SpecialFeatures::NONE},
	{{0, 8, 8}, 4, SpecialFeatures::NONE},
	{{0, 0, 3}, 1, SpecialFeatures::NONE},
};

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
