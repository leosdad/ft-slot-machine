
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "game.h"

// --------------------------------------------------------------- Class members

Game::Game()
{
};

/**
 * Increments the number of coins by the amount given.
 */
uint16_t Game::changeBet(uint16_t bet)
{
	nCoins = min(MAXCOINS, max(0, (signed)(nCoins + bet)));
	// if(!displayDebugInfo) {
	// 	od.SetFont(Font::DIGITS_EXTRALARGE);
	// 	od.PrintN(1, 3, game.nCoins);
	// }
	return nCoins;
}

// ------------------------------------------------------------------------- End
