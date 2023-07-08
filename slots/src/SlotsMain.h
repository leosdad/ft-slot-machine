
#ifndef SLOTS_MAIN_H
#define SLOTS_MAIN_H

// -----------------------------------------------------------------------------

// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "../slots.h"

// ------------------------------------------------------------ Public functions

class SlotsMain
{
	public:
		void Setup();
		void Loop();

	private:
		void processReel(int n, int button);
		void startReels(bool home);
		void calculatePayoff();
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
