
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

// ----------------------------------------------------------------- Debug flags

// Set to true for faster tests
#define SPEEDUP true

// Set to true for reel speed calibration
#define CALIBRATE false

// Set to true to simulate gameplay without actually moving anything
#define SIMULATE false
#define SIMULATE_DELAY	50

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
		uint16_t counter;			// Position counter
		uint16_t finalSteps = 0;	// Steps after sensor is triggered
		uint8_t rotations = 0;		// Rotation counter
		unsigned long lastChange; 	// In microseconds; Used with encoders

		// Must be initialized, don't ever change

		uint8_t encoderPin;			// Encoder motor pin
		uint8_t motorSpeed;			// Motor speed. Motors may behave differently at slow speeds.
		int *composition;			// Reel composition

		// Non-scalar types

		MotorDriver motor{0,0};		// Motor that corresponds to this reel
		ezButton ezHomeSensor{0};	// Initialize to a default pin number (you can change this default if needed)
		ezButton ezLockBtn{0};		// Initialize to a default pin number (you can change this default if needed)
		PwmMicros lockLED;			// Lock LED PWM driver

		// Private member functions

		void lockAndUnlock();

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
			const uint8_t motorSpeedValue,
			int *reelComposition
		);

		uint8_t Start(bool home, uint8_t previousExtraTurns);
		void Reset(bool start);
		void ProcessWhenSpinning();
		void ProcessWhenStopped(bool blinkStatus);
		void Simulate();
		bool IsIdle();
};

// ------------------------------------------------------------------------- End

#endif // REEL_H
