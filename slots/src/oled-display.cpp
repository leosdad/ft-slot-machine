
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "oled-display.h"

// ------------------------------------------------------------------- Variables

const char *symbolNames[NSYMBOLS + 1] = {"All", "Svn", "Ban", "Chr", "Mln", "Bel", "Org", "Lmn", "Grp"};

// ----------------------------------------------------- Public member functions

/**
 * Displays some permanent info on the OLED display.
 */
void OledShow::Setup(bool debug)
{
	odd.Setup();
	debugMode = debug;

	if(debugMode) {
		odd.PrintS(1, 0, "Bt");
		odd.PrintS(1, 5, "Co");
	} else {
		odd.SetFont(Font::MONO_BOLD);
		odd.PrintS(2, 8, "Coins");
		odd.PrintS(3, 8, "Bet");
	}
}

uint16_t OledShow::DisplayCoins(uint16_t number)
{
	if(debugMode) {
		odd.PrintS(1, 8, "    ");
		odd.PrintN(1, 8, number);
	} else {
		odd.SetFont(Font::DIGITS_EXTRALARGE);
		odd.PrintN(1, 3, number);
	}

	return number;
}

uint16_t OledShow::DisplayBet(uint16_t number)
{
	if(debugMode) {
		odd.PrintN(1, 3, number);
	} else {
		odd.SetFont(Font::MONO_BOLD);
		odd.PrintN(3, 12, number);
	}

	return number;
}

// --------------------------------------------------------------- For debugging

/**
 * Shows the state and already drawn symbols of the three reels on the OLED display.
 */
void OledShow::DisplayDebugInfo(Game game)
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
	odd.PrintS(1, 12, "    ");
	odd.PrintN(1, 12, game.spinPayoff * game.currentBet);
#endif

	x = 4 - NPAYLINES;

	for(int l = 0; l < NPAYLINES; l++) {
		odd.PrintS(x + l, 1, symbolNames[payline.GetLineSymbol(l, 0, game.reels[0])]);
		odd.PrintS(x + l, 5, symbolNames[payline.GetLineSymbol(l, 1, game.reels[1])]);
		odd.PrintS(x + l, 9, symbolNames[payline.GetLineSymbol(l, 2, game.reels[2])]);
		odd.PrintS(x + l, 13, "   ");
		odd.PrintN(x + l, 13, game.paylines[l].Payoff);
	}
}

void OledShow::ShowState(const char *str)
{
	if(debugMode) {
		odd.PrintS(0, 0, str);
	}
}

// ------------------------------------------------------------------------- End
