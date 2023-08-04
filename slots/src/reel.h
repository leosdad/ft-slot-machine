
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef REEL_H
#define REEL_H

// -------------------------------------------------------------------- Includes

#include <ezButton.h>
#include <TrueRandom.h>

#include "../slots.h"
#include "motor-driver.h"
#include "pwm-micros.h"

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

		ReelState reelState;		// Machine state
		byte currentSignal;			// Used for encoder debouncing
		unsigned long lastChange; 	// In microseconds; Used with encoders
		uint16_t nSteps;			// Steps counter
		uint16_t finalSteps;		// Steps after sensor is triggered
		uint8_t rotations;			// Rotation counter

		// Must be initialized and won't ever change

		uint8_t encoderPin;			// Encoder motor pin
		uint8_t motorSpeed;			// Motor speed. Motors may behave differently at slow speeds.
		MotorDriver motor{0, 0};	// Motor that spins this reel
		ezButton ezHomeSensor{0};	// ezButton home sensor object
		ezButton ezLockButton{0};	// ezButton lock button object
		PwmMicros lockLED;			// Lock LED PWM driver object

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
		void LoopWhenSpinning();
		void LoopWhenStopped(bool blinkStatus);
		bool IsIdle();
};

// ------------------------------------------------------------------------- End

#endif // REEL_H
