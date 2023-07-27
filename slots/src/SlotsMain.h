
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
	RESETWITHSTART,
	START,
	STARTHOME
};

// ----------------------------------------------------------- Class declaration

class SlotsMain
{
	private:
		bool isIdle();
		void calculateAllPayoffs();
		void blinkLedsTimer();
		void startSpinning(bool home);
		void cmdReels(ReelCommands cmd);
		void ioSetup();

		// Private fields

		Payoffs payoffs;

	public:
		void Setup();
		void Loop();
};

// ------------------------------------------------------------------------- End

#endif // SLOTS_MAIN_H
