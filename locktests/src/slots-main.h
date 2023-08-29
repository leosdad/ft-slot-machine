
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#pragma once

// -------------------------------------------------------------------- Includes

#include <ezButton.h>

// ----------------------------------------------------------- Class declaration

class SlotsMain
{
	private:

		ezButton increaseBet{0};
		ezButton decreaseBet{0};

		void ioSetup();
		void inputLoop();

	public:

		void Setup();
		void Loop();
};

// ------------------------------------------------------------------------- End
