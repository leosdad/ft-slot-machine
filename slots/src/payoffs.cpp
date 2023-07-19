
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
 * @param symbolPosition Array of symbol positions to be displayed (0-11).
 */
uint16_t Payoffs::Calculate(int paylineIndex, uint16_t symbolPosition[])
{
	for(int c = 0; c < NCOMBINATIONS; c++) {

		bool isValidCombination = true;

		for(int i = 0; i < 3; i++) {
			if(payoffTable[c].reel[i] != 0 &&
				payline.GetLineSymbol(paylineIndex, i, symbolPosition) != (int16_t)(payoffTable[c].reel[i])) {
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
