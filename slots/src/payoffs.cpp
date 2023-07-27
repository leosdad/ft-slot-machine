
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "payoffs.h"

// --------------------------------------------------------------- Class members

/**
 * Looks for a payoff combination from the reel positions. If found, returns the
 * respective payoff times the multiplier. If not, returns zero.
 * 
 * @param paylineIndex Index of the desired payline.
 * @param symbolPositions Array of symbol positions to be displayed (0-11).
 */
uint16_t Payoffs::Calculate(int paylineIndex, Reel *reels)
{
	for(int c = 0; c < NCOMBINATIONS; c++) {

		bool isValidCombination = true;

		for(int i = 0; i < NREELS; i++) {
			if(payoffTable[c].symbol[i] != 0 &&
				payline.GetLineSymbol(paylineIndex, i, reels[i]) !=
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

// ------------------------------------------------------------------------- End
