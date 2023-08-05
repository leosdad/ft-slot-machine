
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef PAYOFFS_H
#define PAYOFFS_H

// -------------------------------------------------------------------- Includes

#include "../slots.h"
#include "game.h"

// --------------------------------------------------------------------- Defines

#define spJackpot SpecialFeatures::JACKPOT
#define spNone SpecialFeatures::NONE

// ----------------------------------------------------------- Structs and enums

enum class SpecialFeatures {
	NONE = 0x00,
	JACKPOT = 0x01,
	// BONUS = 0x02,
};

/**
 * Defines winning combinations, payoff values and special features.
 */
struct payoffItem {
	uint8_t symbol[NREELS];
	uint16_t payoff;
	SpecialFeatures features;
};

// ----------------------------------------------------- Global static constants

static const uint8_t payoffMultiplier = PAYMULTIPLIER;

/**
 * 0 (zero) here means any symbol.
 */
static constexpr payoffItem payoffTable[NCOMBINATIONS] = {
	{{1, 1, 1}, 172, spJackpot},
	{{3, 3, 3}, 86, spJackpot},
	{{4, 4, 4}, 86, spJackpot},
	{{5, 5, 3}, 58, spNone},
	{{8, 8, 3}, 43, spNone},
	{{2, 2, 3}, 29, spNone},
	{{8, 8, 8}, 22, spJackpot},
	{{5, 5, 5}, 20, spNone},
	{{2, 2, 2}, 14, spNone},
	{{1, 1, 0}, 14, spNone},
	{{0, 1, 1}, 14, spNone},
	{{1, 0, 1}, 14, spNone},
	{{0, 3, 3}, 14, spNone},
	{{0, 3, 1}, 14, spNone},
	{{0, 4, 4}, 14, spNone},
	{{5, 3, 0}, 14, spNone},
	{{6, 6, 0}, 7, spNone},
	{{3, 3, 0}, 7, spNone},
	{{3, 1, 0}, 7, spNone},
	{{8, 8, 0}, 4, spNone},
	{{0, 8, 8}, 4, spNone},
	{{0, 0, 3}, 1, spNone},
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
