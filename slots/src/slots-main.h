
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#pragma once

// -------------------------------------------------------------------- Includes

#include <ezButton.h>
#include "slots.h"

// ----------------------------------------------------------- Class declaration

class SlotsMain
{
	private:

		bool spinning = false;
		bool lastSpinning = -1;
		ezButton startLever{0};
		ezButton increaseBet{0};
		ezButton decreaseBet{0};

		void ioSetup();
		void inputLoop();

	public:

		void Setup();
		void Loop();
};

// ------------------------------------------------------------------------- End
