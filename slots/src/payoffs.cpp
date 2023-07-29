
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "payoffs.h"

// ---------------------------------------------------- Private member functions

/**
 * Looks for a payoff combination from the reel positions. If found, returns the
 * respective payoff times the multiplier. If not, returns zero.
 * 
 * @param paylineIndex Index of the desired payline.
 * @param symbolPositions Array of symbol positions to be displayed (0-11).
 */
uint16_t Payoffs::calculatePayline(Game game, int paylineIndex)
{
	for(int c = 0; c < NCOMBINATIONS; c++) {

		bool isValidCombination = true;

		for(int i = 0; i < NREELS; i++) {
			if(payoffTable[c].symbol[i] != 0 &&
				payline.GetLineSymbol(paylineIndex, i, game.reels[i]) !=
				(int16_t)(payoffTable[c].symbol[i])) {

				isValidCombination = false;
				break;
			}
		}

		// Found a payoff combination
		if(isValidCombination) {
			return payoffTable[c].payoff * payoffMultiplier;
		}
	}

	return 0;
}

// ----------------------------------------------------- Public member functions

/**
 * Calculates the payoff for all paylines
 */
uint16_t Payoffs::CalculateTotalPayoff(Game *game)
{
	game->spinPayoff = 0;

	for(int l = 0; l < NPAYLINES; l++) {
		game->paylines[l].Payoff = calculatePayline(*game, l);
		game->spinPayoff += game->paylines[l].Payoff;
	}

	if(game->spinPayoff) {
		game->totalWins++;
	}

	return game->spinPayoff;
}

// ------------------------------------------------------------------------- End
