
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef SLOTS_MAIN_H
#define SLOTS_MAIN_H

// -------------------------------------------------------------------- Includes

#include <ezButton.h>
#include "slots.h"

// ----------------------------------------------------------- Class declaration

class SlotsMain
{
	private:

		bool spinning = false;
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

#endif // SLOTS_MAIN_H
