
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "payoffs.h"

// ---------------------------------------------------- Private member functions

/**
 * Looks for a payoff combination from the reel positions. If found, returns the
 * respective payoff times the multiplier. If not, returns zero.
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

		// Found a winning combination: get payoff value and exits
		if(isValidCombination) {
			highestFeature = max(highestFeature, payoffTable[c].feature);
			return payoffTable[c].payoff;
		}
	}

	return 0;
}

// ----------------------------------------------------- Public member functions

/**
 * Sets the value of the multiplier.
 */
bool Payoffs::SetMultiplier(uint8_t multiplier)
{
	return payoffMultiplier = multiplier;
}

/**
 * Returns the highest feature for the latest spin.
 */
SpecialFeatures Payoffs::GetHighestFeature(Game *game)
{
	highestFeature = SpecialFeatures::NONE;
	for(int l = 0; l < NPAYLINES; l++) {
		calculatePayline(*game, l);
	}
	return highestFeature;
}

/**
 * Calculates the payoff for all paylines.
 */
uint16_t Payoffs::CalculateTotalPayoff(Game *game)
{
	game->spinPayoff = 0;
	highestFeature = SpecialFeatures::NONE;

	for(int l = 0; l < NPAYLINES; l++) {
		game->paylines[l].Payoff = calculatePayline(*game, l);
		game->spinPayoff += game->paylines[l].Payoff * game->currentBet * payoffMultiplier;
	}

	return game->spinPayoff;
}

// ------------------------------------------------------------------------- End
