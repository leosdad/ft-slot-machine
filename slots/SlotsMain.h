
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef SLOTS_MAIN_H
#define SLOTS_MAIN_H

// -------------------------------------------------------------------- Includes

#include "slots.h"

// ----------------------------------------------------------------------- Types

enum class ReelStatus {
	IDLE = 0,
	START,
	SENSING,
	COUNTING,
};

// ------------------------------------------------------------ Public functions

class SlotsMain
{
	public:
		void Setup();
		void Loop();

	private:
		bool isIdle();
		uint16_t calcPayoff(int line);
		uint8_t getLineSymbol(uint8_t line, uint8_t reel);
		void changeBet(uint16_t bet);
		void ioSetup();
		void prepareNextSpin(ReelStatus _state);
		void processReel(int n);
		void resetVars();
		void sevenSegSetup();
		void startReels(bool home);

		// Debug

		void forceStopReels();
		void showReelPreview();
};

// ------------------------------------------------------------------------- End

#endif // SLOTS_MAIN_H
