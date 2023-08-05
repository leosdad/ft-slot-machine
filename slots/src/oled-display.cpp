
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "oled-display.h"

// ---------------------------------------------- Global constants and functions

const char *symbolNames[NSYMBOLS + 1] = {"All", "Svn", "Ban", "Chy", "Mln", "Bel", "Org", "Lmn", "Grp"};

uint8_t getSize(uint16_t number)
{
	uint8_t digits = 1;

	while(number >= 10) {
		number /= 10;
		digits++;
	}

	return digits;
}

// ---------------------------------------------------- Private member functions

/**
 * Displays big numbers, right-aligned.
 */
void OledDisplay::displayNumber(uint16_t number, uint8_t xPos, uint8_t maxDigits)
{
	uint8_t size = getSize(number);

	// Erase previous digits if needed

	if(size > 0) {
		odd.SetFont(Font::MONO_BOLD);
		for(int i = 0; i < maxDigits - size; i++) {
			odd.PrintS(2, xPos + i * 2, "  ");
			odd.PrintS(3, xPos + i * 2, "  ");
		}
	}

	odd.SetFont(Font::DIGITS_EXTRALARGE);
	odd.PrintN(2, xPos + (maxDigits - size) * 2, number);
}

// ----------------------------------------------------- Public member functions

/**
 * Displays some permanent info on the OLED display.
 */
void OledDisplay::Setup(bool debug)
{
	odd.Setup();
	debugMode = debug;

	if(debugMode) {
		odd.PrintS(1, 0, "Bt");
		odd.PrintS(1, 5, "Py");
	} else {
		odd.SetFont(Font::MONO_BOLD);
		odd.PrintS(0, 1, "Bet");
		odd.PrintS(0, 9, "Payoff");
	}
}

void OledDisplay::DisplayPayoff(uint16_t payoff)
{
	if(debugMode) {
		odd.PrintS(1, 8, "    ");
		odd.PrintN(1, 8, payoff);
	} else {
		payoff = min(payoff, 9999);
		displayNumber(payoff, 7, 4);
	}
}

void OledDisplay::DisplayBet(uint16_t bet)
{
	if(debugMode) {
		odd.PrintN(1, 3, bet);
	} else {
		displayNumber(bet, 1, 1);
	}
}

/**
 * Shows the state and already drawn symbols of the three reels on the OLED display.
 */
void OledDisplay::DisplayDebugInfo(Game game)
{
	if(!debugMode) {
		return;
	}

	odd.PrintN(0, 10, game.totalSpins);
	odd.PrintN(0, 14, game.totalWins);

#if NPAYLINES == 1
	// uint8_t x = 1;
	// for(int i = 0; i < NREELS; i++, x += 3) {
	// 	odd.PrintS(2, x++, "X");
	// 	odd.PrintN(2, x, game.reels[i].extraTurns);
	// }
	// odd.PrintS(2, 0, "            ");
	odd.PrintN(2, 0, game.nBalls);
	odd.PrintN(2, 5, game.newBall);
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

void OledDisplay::ShowState(const char *str)
{
	if(debugMode) {
		odd.PrintS(0, 0, str);
	}
}

// ------------------------------------------------------------------------- End
