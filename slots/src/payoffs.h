
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef PAYOFFS_H
#define PAYOFFS_H

// -------------------------------------------------------------------- Includes

#include "../slots.h"
#include "game.h"

// ---------------------------------------------------------------------- Struct

struct payoffItem {
	uint8_t symbol[NREELS];
	uint16_t payoff;
};

// ------------------------------------------------------------------- Constants

// Global static constants

static const uint8_t payoffMultiplier = PAYMULTIPLIER;

/**
 * 0 (zero) here means any symbol.
 */
static constexpr payoffItem payoffTable[NCOMBINATIONS] = {
	{{1, 1, 1}, 172},
	{{3, 3, 3}, 86},
	{{4, 4, 4}, 86},
	{{5, 5, 3}, 58},
	{{8, 8, 3}, 43},
	{{2, 2, 3}, 29},
	{{8, 8, 8}, 22},
	{{5, 5, 5}, 20},
	{{2, 2, 2}, 14},
	{{1, 1, 0}, 14},
	{{0, 1, 1}, 14},
	{{1, 0, 1}, 14},
	{{0, 3, 3}, 14},
	{{0, 3, 1}, 14},
	{{0, 4, 4}, 14},
	{{5, 3, 0}, 14},
	{{6, 6, 0}, 7},
	{{3, 3, 0}, 7},
	{{3, 1, 0}, 7},
	{{8, 8, 0}, 4},
	{{0, 8, 8}, 4},
	{{0, 0, 3}, 1},
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
