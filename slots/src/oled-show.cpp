
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "oled-show.h"

// ------------------------------------------------------------------- Variables

const char *symbolNames[NSYMBOLS + 1] = {"All", "Svn", "Ban", "Chr", "Mln", "Bel", "Org", "Lmn", "Grp"};

// -------------------------------------------------------------- Public methods

/**
 * Displays some permanent info on the OLED display.
 */
void OledShow::Setup(bool debug)
{
	odd.Setup();
	debugMode = debug;

	if(!debugMode) {
		odd.SetFont(Font::MONO_BOLD);
		odd.PrintS(2, 8, "Coins");
	}
}

/**
 * Shows the number given as a large number on the OLED display.
 */
void OledShow::DisplayBigNumber(uint16_t number)
{
	if(debugMode) {
		odd.PrintN(1, 1, number);
	} else {
		odd.SetFont(Font::DIGITS_EXTRALARGE);
		odd.PrintN(1, 3, number);
	}
}

// ------------------------------------------------------- Methods for debugging

/**
 * Shows the state and already drawn symbols of the three reels on the OLED display.
 */
void OledShow::ShowReelPreview(Game game)
{
	if(!debugMode) {
		return;
	}

	odd.PrintN(0, 10, game.totalSpins);
	odd.PrintN(0, 14, game.totalWins);

	uint8_t x = 1;

#if NPAYLINES < 3
	// for(int i = 0; i < NREELS; i++, x+=3) {
	// 	odd.PrintS(1, x++, "T");
	// 	odd.PrintN(1, x, game.reels[i].extraTurns);
	// }
	odd.PrintN(1, 1, game.currentBet);
	odd.PrintN(1, 5, game.nCoins);
	odd.PrintN(1, 10, game.spinPayoff);
#endif

	x = 4 - NPAYLINES;

	for(int l = 0; l < NPAYLINES; l++) {
		odd.PrintS(x + l, 1, symbolNames[payline.GetLineSymbol(l, 0, game.reels[0])]);
		odd.PrintS(x + l, 5, symbolNames[payline.GetLineSymbol(l, 1, game.reels[1])]);
		odd.PrintS(x + l, 9, symbolNames[payline.GetLineSymbol(l, 2, game.reels[2])]);
		odd.PrintS(x + l, 13, "   ");
		odd.PrintN(x + l, 13, game.payoff[l]);
	}
}

void OledShow::ShowState(const char *str)
{
	if(debugMode) {
		odd.PrintS(0, 0, str);
	}
}

// ------------------------------------------------------------------------- End
