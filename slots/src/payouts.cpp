
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "payouts.h"

// ---------------------------------------------------- Private member functions

/**
 * Looks for a payout combination from the reel positions. If found, returns the
 * respective payout times the multiplier. If not, returns zero.
 * @param paylineIndex Index of the desired payline.
 * @param symbolPositions Array of symbol positions to be displayed (0-11).
 */
uint16_t Payouts::calculatePayline(Game game, int paylineIndex)
{
	for(int c = 0; c < NCOMBINATIONS; c++) {

		bool isValidCombination = true;

		for(int i = 0; i < NREELS; i++) {
			if(payoutTable[c].symbol[i] != 0 &&
				payline.GetLineSymbol(paylineIndex, i, game.reels[i]) !=
				(int16_t)(payoutTable[c].symbol[i])) {

				isValidCombination = false;
				break;
			}
		}

		// Found a winning combination: get payout value and exits
		if(isValidCombination) {
			highestAward = max(highestAward, payoutTable[c].award);
			return payoutTable[c].payout;
		}
	}

	return 0;
}

// ----------------------------------------------------- Public member functions

/**
 * Sets the value of the multiplier.
 */
bool Payouts::SetMultiplier(uint8_t multiplier)
{
	return payoutMultiplier = multiplier;
}

/**
 * Returns the highest award for the latest spin.
 */
Awards Payouts::GetHighestAward(Game *game)
{
	highestAward = Awards::NONE;

	for(int l = 0; l < NPAYLINES; l++) {
		calculatePayline(*game, l);
	}

	return highestAward;
}

/**
 * Calculates the payout for all paylines.
 */
uint16_t Payouts::CalculateTotalPayout(Game *game)
{
	game->spinPayout = 0;
	highestAward = Awards::NONE;

	for(int l = 0; l < NPAYLINES; l++) {
		game->paylines[l].Payout = calculatePayline(*game, l);
		game->spinPayout += game->paylines[l].Payout * game->currentBet * payoutMultiplier;
	}

	return game->spinPayout;
}

// ------------------------------------------------------------------------- End
