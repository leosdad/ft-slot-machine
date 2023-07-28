
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef SLOTS_MAIN_H
#define SLOTS_MAIN_H

// -------------------------------------------------------------------- Includes

#include "../slots.h"
#include "oled-show.h"
#include "payoffs.h"

// ----------------------------------------------------------------------- Types

enum class ReelCommands {
	IDLE = 0,
	INIT,
	RESET,
	RESETWITHSTART,
	START,
	STARTHOME
};

// ----------------------------------------------------------- Class declaration

class SlotsMain
{
	private:

		bool isIdle();
		void blinkLedsTimer();
		void calculateAllPayoffs();
		void cmdReels(ReelCommands cmd);
		void inputLoops();
		void ioSetup();
		void loopWhenSpinning();
		void loopWhenStopped();
		void startSpinning(bool home);

		// Private fields

		Payoffs payoffs;

	public:
		void Setup();
		void Loop();
};

// ------------------------------------------------------------------------- End

#endif // SLOTS_MAIN_H
