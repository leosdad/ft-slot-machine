
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
		uint8_t currentBet = 0;			// Current bet
		uint16_t nCoins = 0;			// Current number of coins
		uint16_t totalSpins = 0;		// Total spins since the beginning
		uint16_t totalWins = 0;			// Total wins since the beginning
		uint16_t spinPayoff = 0;		// Payoff amount for last spin
		uint16_t payoff[NPAYLINES];		// Payoff for each payline
		Reel *reels;					// Game reels

		void Setup(Reel reels[NREELS]);
		uint8_t SetBet(uint8_t bet);
		uint8_t ChangeBet(int8_t bet);
};

// ------------------------------------------------------------------------- End

#endif // GAME_H
