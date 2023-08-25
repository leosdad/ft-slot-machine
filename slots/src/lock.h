
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#pragma once

// -------------------------------------------------------------------- Includes

#include <ezButton.h>
#include "drivers/pwm-micros.h"

// ----------------------------------------------------------- Class declaration

/**
 * Describes a single lock.
 */
class Lock
{
	private:

		bool locked = false;
		bool lockAllowed = true;
		uint8_t index;
		uint8_t ledPin;
		ezButton ezLockButton{0};	// ezButton lock button object
		PwmMicros pwm;				// Time division pwm object

	public:

		void Setup(uint8_t lockIndex, uint8_t lockButtonPinNumber, uint8_t lockLEDPinNumber);
		bool Loop(bool ledOn);
		void FrozenLoop();
		bool Allow(bool allow);
		bool IsLocked();
		bool IsAllowed();
		void Unlock();
};

// ------------------------------------------------------------------------- End
