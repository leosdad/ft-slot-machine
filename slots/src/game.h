
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
		uint8_t multiplier = 1;
		uint8_t setBet(int8_t bet);
		void init(uint16_t initialCoins);
		void printDebugData(bool home);
		void setupReels();

	public:

		bool playing = false;			// Game status
		bool spinning = false;
		uint8_t doublePay = 0;
		uint16_t spinsLeft = -1;		// Number of spins left in game
		int8_t currentBet = 0;			// Current bet (signed)
		int16_t nCoins = 0;				// Current number of coins (signed)
		uint16_t spinPayout = 0;		// Payout amount for last spin
		Awards lastAward = Awards::NONE;

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
