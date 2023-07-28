
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef SLOTS_MAIN_H
#define SLOTS_MAIN_H

// -------------------------------------------------------------------- Includes

#include "../slots.h"
#include "oled-display.h"
#include "payoffs.h"

// ----------------------------------------------------------------------- Types

enum class ReelCommands {
	IDLE = 0,
	INIT,
};

// ----------------------------------------------------------- Class declaration

class SlotsMain
{
	private:

		void blinkLedsTimer();
		void inputLoop();
		void ioSetup();
		void loopWhenSpinning();
		void loopWhenStopped();
		void startGame(bool home);
		void stopGame();

		// Private fields

		Payoffs payoffs;

	public:
		void Setup();
		void Loop();
};

// ------------------------------------------------------------------------- End

#endif // SLOTS_MAIN_H
