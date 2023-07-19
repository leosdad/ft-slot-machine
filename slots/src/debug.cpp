
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "debug.h"

// --------------------------------------------------------------------- Defines

#define BAUD_RATE 57600

// Macros

#define DISPLAYCHARS	16

// ------------------------------------------------------------------- Variables

const char *symbolNames[NSYMBOLS + 1] = {"All", "Svn", "Ban", "Chr", "Mln", "Bel", "Org", "Lmn", "Grp"};

// -------------------------------------------------------------- Public methods

Debug::Debug()
{
	
}

void Debug::Setup()
{
#if SHOWDEBUGDATA
	od.Setup();
#endif
}

void Debug::DisplayS(const char *msg, uint8_t row = 0, uint8_t col = 0, bool erase = false)
{
#if SHOWDEBUGDATA
	if(erase) {
		od.PrintS(row, 0, "                ");
	}
	od.PrintS(row, col, msg);
#endif
}

void Debug::DisplayN(uint16_t number, uint8_t row = 0, uint8_t col = 0, bool erase = false)
{
#if SHOWDEBUGDATA
	if(erase) {
		od.PrintS(row, 0, "                ");
	}
	od.PrintN(row, col, number);
#endif
}

/**
 * Shows the state and future symbols of the three reels on the OLED display.
 */
void Debug::ShowReelPreview(
	uint16_t totalSpins,
	uint16_t totalWins,
	uint8_t extraTurns[NREELS],
	uint16_t payoff[NREELS],
	uint16_t pos[NREELS]
)
{
#if SHOWDEBUGDATA
	od.PrintN(0, 10, totalSpins);
	od.PrintN(0, 14, totalWins);

	uint8_t x = 1;

#if NPAYLINES < 3
	for(int i = 0; i < NREELS; i++, x+=3) {
		od.PrintS(1, x++, "T");
		od.PrintN(1, x, extraTurns[i]);
	}
#endif

	x = 4 - NPAYLINES;

	for(int l = 0; l < NPAYLINES; l++) {
		od.PrintS(x + l, 1, symbolNames[payline.GetLineSymbol(l, 0, pos)]);
		od.PrintS(x + l, 5, symbolNames[payline.GetLineSymbol(l, 1, pos)]);
		od.PrintS(x + l, 9, symbolNames[payline.GetLineSymbol(l, 2, pos)]);
		od.PrintS(x + l, 13, "   ");
		od.PrintN(x + l, 13, payoff[l]);
	}
#endif
}

// ------------------------------------------------------------------------- End
