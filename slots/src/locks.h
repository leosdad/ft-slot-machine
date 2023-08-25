
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#pragma once

// -------------------------------------------------------------------- Includes

#include "lock.h"

// ----------------------------------------------------------- Class declaration

class Locks
{
	private:

		// bool allowNext;
		// bool preventNext;
		uint8_t lastLocked = -1;
		uint8_t lastMaxLockable = -1;
		uint8_t lastBetValue = -1;

		void Locks::debug(uint8_t currentlyLocked, uint8_t maxLockable);
		// void calculate();

	public:

		bool ledState = false;
		void Setup();
		// void Freeze();
		void Loop(bool isSpinning);
};

// ------------------------------------------------------------------------- End
