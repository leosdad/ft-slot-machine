
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef SLOTS_MAIN_H
#define SLOTS_MAIN_H

// -------------------------------------------------------------------- Includes

#include "../slots.h"
#include "debug.h"
#include "oled-display.h"
#include "payoffs.h"

// ----------------------------------------------------------------------- Types

enum class ReelStatus {
	IDLE = 0,
	START,
	SENSING,
	COUNTING,
};

enum class GameStatus {
	IDLE = 0,
	IDLE_SPINNING,
	PLAYING,
	PLAYING_SPINNING,
};

// ----------------------------------------------------------- Class declaration

class SlotsMain
{
	public:
		void Setup();
		void Loop();

	private:
		bool isIdle();
		void ioSetup();
		void prepareNextSpin(ReelStatus _state);
		void resetVars();
		void sevenSegSetup();
		void __unnamed();
		void forceStopReels();
		void blinkLedsTimer();

		void displayBet();

		// Private fields

		Debug debug;
		OledDisplay od;
		Payline payline;
		Payoffs payoffs;
};

// ------------------------------------------------------------------------- End

#endif // SLOTS_MAIN_H
