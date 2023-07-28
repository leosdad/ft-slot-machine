
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "game.h"

// --------------------------------------------------------------- Class members

void Game::Setup(Reel myReels[NREELS])
{
	reels = myReels;

	for(int l = 0; l < NPAYLINES; l++) {
		payoff[l] = 0;
	}
}

/**
 * Sets the bet to the amount given.
 */
uint8_t Game::SetBet(uint8_t bet)
{
	currentBet = constrain(bet, 0, MAXCOINS);
	return currentBet;
}

/**
 * Increments the bet by the amount given.
 */
uint8_t Game::ChangeBet(int8_t bet)
{
	currentBet = constrain(currentBet + bet, 0, MAXCOINS);
	return currentBet;
}

// ------------------------------------------------------------------------- End
