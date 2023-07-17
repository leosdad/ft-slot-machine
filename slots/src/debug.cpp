
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#pragma region -------------------------------------------------------- Includes

#include "debug.h"
#include "oled-display.h"

#pragma endregion

#pragma region --------------------------------------------------------- Defines

#define BAUD_RATE 57600

// Macros

// TODO: repeated code from SlotsMain.cpp

#define CALCN1(n)	(n > 0 ? n - 1 : n + NREELSYMBOLS - 1)
#define CALCN2(n)	(n)
#define CALCN3(n)	(n < NREELSYMBOLS - 1 ? n + 1 : 0 )

// Paylines

#define LINE1(n)	(reels[n][CALCN1(pos[n])])
#define LINE2(n)	(reels[n][CALCN2(pos[n])])
#define LINE3(n)	(reels[n][CALCN3(pos[n])])

#define DISPLAYCHARS	16

#pragma endregion

#pragma region ------------------------------------------------------- Variables

const char *symbolNames[NSYMBOLS + 1] = {"All", "Svn", "Ban", "Chr", "Mln", "Bel", "Org", "Lmn", "Grp"};

#pragma endregion --------------------------------------------------------------

#pragma region -------------------------------------------------- Public methods

void Debug::Setup()
{
#if SHOWDEBUGDATA
	oledSetup();
#endif
}

void Debug::DisplayS(const char *msg, uint8_t row = 0, uint8_t col = 0, bool erase = false)
{
#if SHOWDEBUGDATA
	if(erase) {
		oledPrintS(row, 0, "                ");
	}
	oledPrintS(row, col, msg);
#endif
}

void Debug::DisplayN(uint16_t number, uint8_t row = 0, uint8_t col = 0, bool erase = false)
{
#if SHOWDEBUGDATA
	if(erase) {
		oledPrintS(row, 0, "                ");
	}
	oledPrintN(row, col, number);
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
	oledPrintN(0, 10, totalSpins);
	oledPrintN(0, 14, totalWins);

	uint8_t x = 1;

#if NPAYLINES < 3
	for(int i = 0; i < NREELS; i++, x+=3) {
		oledPrintS(1, x++, "T");
		oledPrintN(1, x, extraTurns[i]);
	}
#endif

	x = 4 - NPAYLINES;

	for(int l = 0; l < NPAYLINES; l++) {
		oledPrintS(x + l, 1, symbolNames[getLineSymbol(l, 0, pos)]);
		oledPrintS(x + l, 5, symbolNames[getLineSymbol(l, 1, pos)]);
		oledPrintS(x + l, 9, symbolNames[getLineSymbol(l, 2, pos)]);
		oledPrintS(x + l, 13, "   ");
		oledPrintN(x + l, 13, payoff[l]);
	}
#endif
}

#pragma endregion

#pragma region ------------------------------------------------- Private methods

// TODO: repeated code from SlotsMain.cpp

/**
 * Returns the current symbol number of the line and reel specified.
 */
uint8_t Debug::getLineSymbol(uint8_t line, uint8_t reel, uint16_t pos[NREELS])
{
	switch(line) {
		case 0:
			return LINE1(reel);
		case 1:
			return LINE2(reel);
		case 2:
			return LINE3(reel);
	}
}

#pragma endregion
