
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef SLOTS_MAIN_H
#define SLOTS_MAIN_H

// -------------------------------------------------------------------- Includes

#include "../slots.h"

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
		void stopReels();
		void displayIdle(char *msg);
		bool isIdle();
		void resetVars(int _state);
		void serialSetup();
		void sevenSegSetup();
		void ioSetup();
		void showReelPreview();

};

// ------------------------------------------------------------------------- End

#endif // SLOTS_MAIN_H
