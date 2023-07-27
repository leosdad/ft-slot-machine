
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "game.h"

// --------------------------------------------------------------- Class members

/**
 * Increments the number of coins by the amount given.
 */
uint16_t Game::SetBet(uint16_t bet)
{
	nCoins = constrain(bet, 0, MAXCOINS);
	// if(!displayDebugInfo) {
	// 	od.SetFont(Font::DIGITS_EXTRALARGE);
	// 	od.PrintN(1, 3, game.nCoins);
	// }
	return nCoins;
}


/**
 * Increments the number of coins by the amount given.
 */
uint16_t Game::ChangeBet(uint16_t bet)
{
	nCoins = constrain(nCoins + bet, 0, MAXCOINS);
	// if(!displayDebugInfo) {
	// 	od.SetFont(Font::DIGITS_EXTRALARGE);
	// 	od.PrintN(1, 3, game.nCoins);
	// }
	return nCoins;
}

// ------------------------------------------------------------------------- End
