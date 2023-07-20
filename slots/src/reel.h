
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef REEL_H
#define REEL_H

// -------------------------------------------------------------------- Includes

#include "../slots.h"
#include <ezButton.h>
#include <TrueRandom.h>
#include <MotorDriver.h>

// ----------------------------------------------------------- Class declaration

class Reel
{
	private:

		// enum class LockableStatus {
		// 	LOCKABLE = 0,
		// 	NOT_LOCKABLE,
		// 	NEXT_LOCABLE
		// };

		enum class ReelState {
			IDLE = 0,
			START,
			SENSING,
			COUNTING,
		};

		ReelState reelState;		// Machine state
		bool lockable = true;		// Signals if the reel can be locked
		bool locked = false;		// State of reel lock
		byte currentSignal;			// Used for encoder debouncing
		uint16_t counter;			// Position counter
		uint16_t finalSteps = 0;	// Steps after sensor is triggered
		uint16_t symbolPos = 0;		// Position of symbol to be displayed (0-11)
		uint8_t extraTurns = 0;		// Number of extra 360° revolutions
		uint8_t rotations;			// Rotation counter
		unsigned long lastChange; 	// In microseconds; Used with encoders

		// Not modified, must be initialized

		uint8_t motorPins[2];		// Motor pins
		uint8_t encoderPin;			// Encoder motor pin
		uint8_t homeSensorPin;		// Pin for optic home position sensor
		uint8_t lockLEDPin;			// Pin for green lock reel LED
		uint8_t lockButtonPin;		// Pin for pushbutton behind LED
		uint8_t motorSpeed;			// Motor speed. Motors may behave differently at slow speeds.
		MotorDriver &motor;			// Motor that corresponds to this reel
		ezButton &ezHomeSensor;		// ezButton instance for home sensor
		ezButton &ezLockBtn;		// ezButton instance for reel lock button
		int *composition;			// Reel composition

	public:

		/**
		 * Constructor.
		*/
		Reel(
			const uint8_t motorPinNumbers[2],
			const uint8_t encoderPinNumber,
			const uint8_t homeSensorPinNumber,
			const uint8_t lockButtonPinNumber,
			const uint8_t lockLEDPinNumber,
			const uint8_t motorSpeedValue,
			int *reelComposition
		);

		void Setup();
		uint8_t Start(bool home, uint8_t previousExtraTurns);
		void Process();
		void Simulate();
		bool IsIdle();
		void ForceStop();
};

// ------------------------------------------------------------------------- End

#endif // REEL_H
