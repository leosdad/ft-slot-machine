
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef GAME_H
#define GAME_H

// -------------------------------------------------------------------- Includes

#include "slots.h"
#include "reel.h"
#include "payline.h"

// ----------------------------------------------------------- Class declaration

class Game
{
	private:

		bool playing = false;			// Game status
		uint8_t setBet(int8_t bet);
		void init();
		void stopSpin();
		void printDebugData(bool home);
		void setupReels(
			const uint8_t motorPins[NREELS][2],
			const uint8_t encoderPin[NREELS],
			const uint8_t homeSensorPin[NREELS],
			const uint8_t lockButtonPin[NREELS],
			const uint8_t lockLEDPinNumber[NREELS],
			const uint8_t motorSpeedValue[NREELS]
		);

	public:

		// Variables

		uint16_t totalSpins = 0;		// Total spins since the beginning
		uint16_t totalWins = 0;			// Total wins since the beginning
		int8_t currentBet = 0;			// Current bet (signed)
		int16_t nCoins = 0;				// Current number of coins (signed)
		uint16_t spinPayoff = 0;		// Payoff amount for last spin
		uint8_t nBalls = 0;				// Number of balls won
		bool newBall = false;			// Activated when a new ball is won
		Reel reels[NREELS];				// Game reels
		Payline paylines[NPAYLINES];	// Game paylines

		// Member functions

		void Setup();
		bool Loop();
		void StartSpin(bool home);
		uint8_t ChangeBet(int8_t bet);
};

// ------------------------------------------------------------------------- End

#endif // GAME_H
