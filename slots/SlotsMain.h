
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef SLOTS_MAIN_H
#define SLOTS_MAIN_H

// -------------------------------------------------------------------- Includes

#include "slots.h"

// ----------------------------------------------------------------------- Types

typedef enum {
	IDLE = 0,
	START,
	SENSING,
	COUNTING,
} ReelStatus;

// ------------------------------------------------------------ Public functions

class SlotsMain
{
	public:
		void Setup();
		void Loop();

	private:
		void processReel(int n);
		void startReels(bool home);
		uint8_t getLineSymbol(uint8_t line, uint8_t reel);
		uint16_t calcPayoff(int line);
		void resetVars();
		bool isIdle();
		void prepareNextSpin(ReelStatus _state);
		void sevenSegSetup();
		void ioSetup();

		// Debug

		void serialSetup();
		void showReelPreview();
		void forceStopReels();
};

// ------------------------------------------------------------------------- End

#endif // SLOTS_MAIN_H
