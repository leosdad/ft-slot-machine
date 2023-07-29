
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "oled-display.h"
// #include <iostream>
#include <string.h>

// ------------------------------------------------------------ Global constants

const char *symbolNames[NSYMBOLS + 1] = {"All", "Svn", "Ban", "Chy", "Mln", "Bel", "Org", "Lmn", "Grp"};

// ---------------------------------------------------- Private member functions

uint8_t OledShow::getSize(uint8_t number)
{
	uint8_t digits = 1;

	while(number >= 10) {
		number /= 10;
		digits++;
	}

	return digits;
}

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
		odd.PrintS(2, 11, "Coins");
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
		odd.PrintS(1, 2, "    ");
		odd.PrintN(1, 2, number);
		// odd.PrintN(1, 8 * 3 - 8 * getSize(number), number);
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

#if NPAYLINES == 1
	uint8_t x = 1;
	for(int i = 0; i < NREELS; i++, x += 3) {
		odd.PrintS(2, x++, "X");
		odd.PrintN(2, x, game.reels[i].extraTurns);
	}
#endif

#if NPAYLINES < 3
	odd.PrintS(1, 13, "    ");
	odd.PrintN(1, 13, game.spinPayoff * game.currentBet);
#endif

	uint8_t y = 4 - NPAYLINES;

	for(int l = 0; l < NPAYLINES; l++) {
		odd.PrintS(y + l, 1, symbolNames[payline.GetLineSymbol(l, 0, game.reels[0])]);
		odd.PrintS(y + l, 5, symbolNames[payline.GetLineSymbol(l, 1, game.reels[1])]);
		odd.PrintS(y + l, 9, symbolNames[payline.GetLineSymbol(l, 2, game.reels[2])]);
		odd.PrintS(y + l, 13, "   ");
		odd.PrintN(y + l, 13, game.paylines[l].Payoff);
	}
}

void OledShow::ShowState(const char *str)
{
	if(debugMode) {
		odd.PrintS(0, 0, str);
	}
}

// ------------------------------------------------------------------------- End
