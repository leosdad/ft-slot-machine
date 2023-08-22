
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#pragma once

// -------------------------------------------------------------------- Includes

#include <ezButton.h>
#include <ezLED.h>

// ----------------------------------------------------------- Class declaration

/** Describes a single lock. */
class Lock
{
	private:

		bool locked = false;
		bool lockAllowed = true;

		ezButton ezLockButton{0};	// ezButton lock button object
		ezLED ezLockLED{0};			// ezLED lock LED object

	public:

		void Setup(uint8_t lockButtonPinNumber, uint8_t lockLEDPinNumber);
		bool Loop(bool ledOn);
		bool Allow(bool allow);
		bool Locked();
		bool Allowed();
		void Unlock();
};

// ------------------------------------------------------------------------- End
