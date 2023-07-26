
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef GAME_H
#define GAME_H

// -------------------------------------------------------------------- Includes

#include "../slots.h"

// --------------------------------------------------------------------- Defines

#define STARTCOINS		3
#define MAXCOINS		9

// ----------------------------------------------------------- Class declaration

class Game
{
	private:


	public:


		// Number of initial coins
		// const uint16_t startCoins = 3;
		// const uint16_t maxCoins = 9;
		bool playing = false;			// Game status
		bool spinning = false;			// Game status

		uint16_t nCoins = STARTCOINS;	// Current number of coins
		uint16_t totalSpins = 0;		// Total spins since the beginning
		uint16_t totalWins = 0;			// Total wins since the beginning

		Game();

		uint16_t changeBet(uint16_t bet);
};

// ------------------------------------------------------------------------- End

#endif // GAME_H
