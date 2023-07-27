
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "small-display.h"

// --------------------------------------------------------------- Class members

/**
 * Shows some permanent info on the OLED display.
 */
void SmallDisplay::Setup(bool show)
{
	od.Setup();
	displayDebugInfo = show;
	if(!show) {
		od.SetFont(Font::MONO_BOLD);
		od.PrintS(2, 8, "Coins");
	}
}

/**
 * Shows the current bet amount on the OLED display.
 */
void SmallDisplay::DisplayBet(uint16_t nCoins)
{
	if(!displayDebugInfo) {
		od.SetFont(Font::DIGITS_EXTRALARGE);
		od.PrintN(1, 3, nCoins);
	}
}

// ------------------------------------------------------------------------- End
