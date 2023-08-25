
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef CHEERING_H
#define CHEERING_H

// -------------------------------------------------------------------- Includes

#include <ezLED.h>
#include "slots.h"

// ----------------------------------------------------------- Class declaration

class Cheering
{
	private:

		bool isFadedIn = false;
		ezLED leftSignal{0};
		ezLED rightSignal{0};

	public:

		void Start();
		void Loop(bool enable, bool cheerALot);
		void Stop();
};

// ------------------------------------------------------------------------- End

#endif // CHEERING_H
