
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#pragma once

// -------------------------------------------------------------------- Includes

#include "slots.h"
#include "reel.h"
#include "payline.h"

// ----------------------------------------------------------- Class declaration

class Game
{
	private:

		bool lastSpinning = -1;

		uint8_t setBet(int8_t bet);
		void init(uint16_t initialCoins);
		void printDebugData(bool home);
		void setupReels();

	public:

		bool playing = false;			// Game status
		bool spinning = false;
		bool doublePay = false;
		uint16_t totalWins = 0;			// Total wins since the beginning
		uint16_t totalSpins = 0;		// Total spins since the beginning
		int8_t currentBet = 0;			// Current bet (signed)
		int16_t nCoins = 0;				// Current number of coins (signed)
		uint16_t spinPayoff = 0;		// Payoff amount for last spin
		SpecialFeatures lastFeature = SpecialFeatures::NONE;

		Reel reels[NREELS];				// Game reels
		Payline paylines[NPAYLINES];	// Game paylines

		void Setup(uint16_t initialCoins);
		void SetCoins(uint16_t nCoins);
		bool Loop();
		bool StartSpin(bool home);
		uint8_t ChangeBet(int8_t bet = 0);
		bool BounceReelsBack();
		bool BounceReelsForward();
};

// ------------------------------------------------------------------------- End
