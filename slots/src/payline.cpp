
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "payline.h"

#pragma region --------------------------------------------------------- Defines

// Macros

#define CALCN1(l)	(l > 0 ? l - 1 : l + NREELSYMBOLS - 1)
#define CALCN2(l)	(l)
#define CALCN3(l)	(l < NREELSYMBOLS - 1 ? l + 1 : 0 )

// Payline symbols

#define LINE1(l, pos)	(reels[l][CALCN1(pos[l])])
#define LINE2(l, pos)	(reels[l][CALCN2(pos[l])])
#define LINE3(l, pos)	(reels[l][CALCN3(pos[l])])

#pragma endregion

// --------------------------------------------------------------- Class members

/**
 * Returns the current symbol number of the line and reel specified.
 */
uint8_t Payline::GetLineSymbol(uint8_t line, uint8_t reel, uint16_t pos[])
{
	switch(line) {
		case 0:
			return LINE1(reel, pos);
		case 1:
			return LINE2(reel, pos);
		case 2:
			return LINE3(reel, pos);
	}
}

// ------------------------------------------------------------------------- End
