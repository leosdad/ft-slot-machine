
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
void OledShow::Setup(bool show)
{
	odd.Setup();
	displayDebugInfo = show;

	if(!displayDebugInfo) {
		odd.SetFont(Font::MONO_BOLD);
		odd.PrintS(2, 8, "Coins");
	}
}

/**
 * Shows the current bet amount on the OLED display.
 */
void OledShow::DisplayBet(uint16_t nCoins)
{
	if(!displayDebugInfo) {
		odd.SetFont(Font::DIGITS_EXTRALARGE);
		odd.PrintN(1, 3, nCoins);
	}
}

// ---------------------------------------------------------- Debug mode methods

/**
 * Shows the state and already drawn symbols of the three reels on the OLED display.
 */
void OledShow::ShowReelPreview(Game game, Reel reels[NREELS], uint16_t payoff[NPAYLINES])
{
	if(!displayDebugInfo) {
		return;
	}

	odd.PrintN(0, 10, game.totalSpins);
	odd.PrintN(0, 14, game.totalWins);

	uint8_t x = 1;

#if NPAYLINES < 3
	for(int i = 0; i < NREELS; i++, x+=3) {
		odd.PrintS(1, x++, "T");
		odd.PrintN(1, x, reels[i].extraTurns);
	}
#endif

	x = 4 - NPAYLINES;

	for(int l = 0; l < NPAYLINES; l++) {
		odd.PrintS(x + l, 1, symbolNames[payline.GetLineSymbol(l, 0, reels[0])]);
		odd.PrintS(x + l, 5, symbolNames[payline.GetLineSymbol(l, 1, reels[1])]);
		odd.PrintS(x + l, 9, symbolNames[payline.GetLineSymbol(l, 2, reels[2])]);
		odd.PrintS(x + l, 13, "   ");
		odd.PrintN(x + l, 13, payoff[l]);
	}
}

void OledShow::ShowState(const char *str)
{
	if(displayDebugInfo) {
		odd.PrintS(0, 0, str);
	}
}

// ------------------------------------------------------------------------- End
