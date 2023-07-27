
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef GAME_H
#define GAME_H

// -------------------------------------------------------------------- Includes

#include "../slots.h"

// --------------------------------------------------------------------- Defines


// ----------------------------------------------------------- Class declaration

class Game
{
	public:

		// Number of initial coins
		// const uint16_t startCoins = 3;
		// const uint16_t maxCoins = 9;
		bool playing = false;			// Game status
		bool spinning = false;			// Game status

		uint16_t nCoins = 0;			// Current number of coins
		uint16_t totalSpins = 0;		// Total spins since the beginning
		uint16_t totalWins = 0;			// Total wins since the beginning

		uint16_t SetBet(uint16_t bet);
		uint16_t ChangeBet(uint16_t bet);
};

// ------------------------------------------------------------------------- End

#endif // GAME_H
