
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef CHEERING_H
#define CHEERING_H

// -------------------------------------------------------------------- Includes

#include "../slots.h"
#include "game.h"

// ----------------------------------------------------------- Class declaration

class Cheering
{
	private:

		unsigned long blinkPreviousMs;
		bool allowNext;

	public:

		int blinkLedState;

		Cheering();

		void Loop(Game *game);
		void Stop();
};

// ------------------------------------------------------------------------- End

#endif // CHEERING_H
