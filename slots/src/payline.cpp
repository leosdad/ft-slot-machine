
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "payline.h"

#pragma region --------------------------------------------------------- Defines

// Macros

#define CALCN1(l)	(l > 0 ? l - 1 : l + NREELSYMBOLS - 1)
#define CALCN2(l)	(l)
#define CALCN3(l)	(l < NREELSYMBOLS - 1 ? l + 1 : 0 )

#pragma endregion

// --------------------------------------------------------------- Class members

/**
 * Returns the current symbol number of the line and reel specified.
 */
uint8_t Payline::GetLineSymbol(uint8_t line, uint8_t nReel, Reel reel)
{
	const int *symbols = reelComposition[line];

	switch(line) {
		case 0:
			return symbols[CALCN1(reel.symbolPos)];
		case 1:
			return symbols[CALCN2(reel.symbolPos)];
		case 2:
			return symbols[CALCN3(reel.symbolPos)];
	}
}

// ------------------------------------------------------------------------- End
