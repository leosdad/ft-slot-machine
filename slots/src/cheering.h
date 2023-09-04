
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#pragma once

// -------------------------------------------------------------------- Includes

#include <ezLED.h>
#include "slots.h"

enum class CheerLevel {
	NONE = 0,
	DRAW,
	WIN,
	BIG_WIN
};

// ----------------------------------------------------------- Class declaration

class Cheering
{
	private:

		CheerLevel cheerLevel;
		bool firstCycle = true;
		bool active = false;
		uint8_t fadeCycles;

		ezLED leftSignal{0};
		ezLED rightSignal{0};

	public:

		void Setup();
		void Loop(bool enable);
		void Start(CheerLevel cheerLevel);
		void Stop();
};

// ------------------------------------------------------------------------- End
