
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "payline.h"

// ---------------------------------------------------------------------- Macros

// Symbol positions

#define POS1(n)	(n > 0 ? n - 1 : n + NREELSYMBOLS - 1)
#define POS2(n)	(n)
#define POS3(n)	(n < NREELSYMBOLS - 1 ? n + 1 : 0 )

// Paylines

#define LINE1(n)	(reelComposition[n][POS1(reel.symbolPos)])
#define LINE2(n)	(reelComposition[n][POS2(reel.symbolPos)])
#define LINE3(n)	(reelComposition[n][POS3(reel.symbolPos)])

// ----------------------------------------------------- Public member functions

uint8_t Payline::GetLineSymbol(uint8_t line, uint8_t nReel, Reel reel)
{
	#if NPAYLINES == 1
		return LINE2(nReel);
	#else
		switch(line) {
			case 0:
				return LINE1(nReel);
			case 1:
				return LINE2(nReel);
			case 2:
				return LINE3(nReel);
		}
	#endif
}

// ------------------------------------------------------------------------- End
