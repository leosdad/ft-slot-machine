
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
	PROCESSSPINNING,
	PROCESSSTOPPED,
	RESET,
	START,
};

// ----------------------------------------------------------- Class declaration

class SlotsMain
{
	private:
		bool isIdle();
		void __unnamed();
		void blinkLedsTimer();
		void cmdReels(ReelCommands cmd);
		void ioSetup();

		// Private fields

		Payline payline;
		Payoffs payoffs;

	public:
		void Setup();
		void Loop();
};

// ------------------------------------------------------------------------- End

#endif // SLOTS_MAIN_H
