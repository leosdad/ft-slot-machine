
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef SLOTS_MAIN_H
#define SLOTS_MAIN_H

// -------------------------------------------------------------------- Includes

#include "../slots.h"
#include "oled-display.h"
#include "payoffs.h"

// ----------------------------------------------------------- Class declaration

class SlotsMain
{
	private:

		void inputLoop();
		void ioSetup();
		void loopWhenSpinning();
		void loopWhenStopped();
		void startSpin(bool home);
		void stopSpin();

	public:
		void Setup();
		void Loop();
};

// ------------------------------------------------------------------------- End

#endif // SLOTS_MAIN_H
