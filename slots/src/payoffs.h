
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef PAYOFFS_H
#define PAYOFFS_H

// -------------------------------------------------------------------- Includes

#include "../slots.h"
#include "payline.h"
#include "reel.h"

// --------------------------------------------------------------------- Defines

#define NCOMBINATIONS	22

// -------------------------------------------------------------000------ Struct

/**
 * Each line is: reel 0, reel 1, reel 2, payoff. 0 on a reel means "any".
 */
struct payoffItem {
	uint8_t symbol[NREELS];
	uint16_t payoff;
};

// ------------------------------------------------------------------- Constants

// Static constants

static const uint8_t payoffMultiplier = 3;
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
	public:
		uint16_t Calculate(int paylineIndex, Reel *reels);

	private:
		Payline payline;
};

// ------------------------------------------------------------------------- End

#endif // PAYOFFS_H
