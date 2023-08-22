
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#pragma once

// -------------------------------------------------------------------- Includes

#include <ezButton.h>
#include <ezLED.h>
#include <TrueRandom.h>

#include "slots.h"
#include "drivers/motor-driver.h"

// ----------------------------------------------------------- Class declaration

class Reel
{
	private:

		// Enums

		enum class ReelState {
			IDLE = 0,
			// WAITING,
			START,
			SENSING,
			COUNTING,
		};

		// Private fields

		ReelState reelState = ReelState::IDLE;	// Machine state
		byte currentSignal;			// Used for debouncing
		unsigned long lastChange; 	// Used for debouncing
		uint16_t nSteps;			// Steps counter
		uint16_t finalSteps;		// Steps after sensor is triggered
		uint8_t rotations;			// Rotation counter
		bool lastLockedValue = -1;

		// Must be initialized and won't ever change

		uint8_t encoderPin;			// Encoder motor pin
		uint8_t homePin;			// Home sensor pin
		uint8_t motorSpeed;			// Motor speed. Motors may behave differently at slow speeds.
		MotorDriver motor{0};		// Motor that spins this reel
		ezButton ezLockButton{0};	// ezButton lock button object
		ezLED ezLockLED{0};			// ezLED lock LED object

		bool idle();
		bool start();
		bool sensing();
		bool counting();

	public:

		// Variables

		uint16_t symbolPos = 0;		// Position of symbol to be displayed (0-11)
		uint8_t extraTurns = 0;		// Number of extra 360° revolutions
		bool lockable = true;		// Signals if the reel can be locked
		bool locked = false;		// State of reel lock

		// Public member functions

		void Setup(
			const uint8_t motorPins[2],
			const uint8_t encoderPin,
			const uint8_t homeSensorPin,
			const uint8_t lockButtonPin,
			const uint8_t lockLEDPinNumber,
			const uint8_t motorSpeedValue
		);
		uint8_t Start(bool home, uint8_t previousExtraTurns);
		bool Loop();
		// bool IsIdle();
};

// ------------------------------------------------------------------------- End
