
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef GAME_H
#define GAME_H

// -------------------------------------------------------------------- Includes

#include "../slots.h"
#include "reel.h"

// ----------------------------------------------------------- Class declaration

class Game
{
	public:

		bool playing = false;			// Game status
		bool spinning = false;			// Reels spinning status
		uint16_t nCoins = 0;			// Current number of coins
		uint16_t totalSpins = 0;		// Total spins since the beginning
		uint16_t totalWins = 0;			// Total wins since the beginning
		Reel *reels;

		void Setup(Reel reels[NREELS]);
		uint16_t SetBet(uint16_t bet);
		uint16_t ChangeBet(uint16_t bet);
};

// ------------------------------------------------------------------------- End

#endif // GAME_H
