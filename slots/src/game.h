
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef GAME_H
#define GAME_H

// -------------------------------------------------------------------- Includes

#include "../slots.h"
#include "reel.h"
#include "payline.h"

// ----------------------------------------------------------- Class declaration

class Game
{
	public:

		// Variables

		bool playing = false;			// Game status
		bool spinning = false;			// Reels spinning status
		uint8_t currentBet = 0;			// Current bet
		uint16_t nCoins = 0;			// Current number of coins
		uint16_t totalSpins = 0;		// Total spins since the beginning
		uint16_t totalWins = 0;			// Total wins since the beginning
		uint16_t spinPayoff = 0;		// Payoff amount for last spin
		Reel reels[NREELS];				// Game reels
		Payline paylines[NPAYLINES];	// Game paylines

		// Member functions

		void Setup(Reel reels[NREELS]);
		uint8_t SetBet(uint8_t bet);
		uint8_t ChangeBet(int8_t bet);
		void ProcessWhenSpinning();
		void ProcessWhenStopped(int blinkLedState);
		void StartReels(bool home);
		void ResetReels(bool start);
		bool IsIdle();
		void InitReels(
			const uint8_t motorPins[NREELS][2],
			const uint8_t encoderPin[NREELS],
			const uint8_t homeSensorPin[NREELS],
			const uint8_t lockButtonPin[NREELS],
			const uint8_t lockLEDPinNumber[NREELS],
			const uint8_t motorSpeedValue[NREELS],
			int **reelComposition
		);
};

// ------------------------------------------------------------------------- End

#endif // GAME_H
