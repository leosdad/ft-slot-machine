
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#pragma once

// -------------------------------------------------------------------- Includes

#include "drivers/motor-driver.h"

// ----------------------------------------------------------- Class declaration

class Reel
{
	private:

		// Enums

		enum class ReelState {
			IDLE = 0,
			START,
			SENSING,
			COUNTING,
		};

		// Private fields

		ReelState reelState = ReelState::IDLE;	// Machine state
		byte currentSignal;			// Used for debouncing
		unsigned long lastChange; 	// Used for debouncing
		uint8_t index;				// Reel index
		uint16_t nSteps;			// Steps counter
		uint16_t finalSteps;		// Steps after sensor is triggered
		uint8_t rotations;			// Rotation counter

		// Must be initialized and won't ever change

		uint8_t encoderPin;			// Encoder motor pin
		uint8_t homePin;			// Home sensor pin
		uint8_t motorSpeed;			// Motor speed. Motors may behave differently at slow speeds.
		MotorDriver motor{0};		// Motor that spins this reel

		bool idle();
		bool start();
		bool sensing();
		bool counting();

	public:

		// Variables

		uint16_t symbolPos = 0;		// Position of symbol to be displayed (0-11)
		uint8_t extraTurns = 0;		// Number of extra 360° revolutions

		// Public member functions

		void Setup(
			const uint8_t index,
			const uint8_t motorPins[2],
			const uint8_t encoderPin,
			const uint8_t homeSensorPin,
			const uint8_t motorSpeedValue
		);
		uint8_t Start(bool home, uint8_t previousExtraTurns);
		bool Loop();
};

// ------------------------------------------------------------------------- End
