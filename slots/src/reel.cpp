
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "reel.h"

#include <TrueRandom.h>
#include <ezButton.h>

#include "locks.h"
#include "slots.h"

// -------------------------------------------------------------------- Defines

#define DEBUGREELS	false

// ------------------------------------------------------------ Global variables

extern Locks locks;

// ---------------------------------------------------- Private member functions

bool Reel::idle()
{
	// Does nothing
}

bool Reel::start()
{
	rotations = extraTurns;
	reachedHome = false;
	#if DEBUGREELS
	Serial.println("  Reel #" + String(index) + " started with " + String(rotations) + " rotations");
	#endif
	motor.RotateCW(normalSpeed);
	reelState = ReelState::SENSING;
}

/**
 * Decrements rotation counter and waits until the reel reaches the home position.
 */
bool Reel::sensing()
{
	if(micros() - lastHome > HOMEDEBOUNCE) {

		bool isHome = digitalRead(homePin);

		if(isHome && !reachedHome) {
			lastHome = micros();
			reachedHome = true;

			if(rotations == 0) {
				nSteps = finalSteps;
				pulseSignal = digitalRead(encoderPin);
				#if DEBUGREELS
				Serial.println("  Reel #" + String(index) + " slowed down");
				#endif
				motor.RotateCW(slowSpeed);
				reelState = ReelState::COUNTING;
			} else if(rotations > 0) {
				rotations--;
				#if DEBUGREELS
				Serial.println("  Reel #" + String(index) + ": " + String(rotations) + " rotations");
				#endif
			}

		} else if(!isHome) {

			reachedHome = false;

		}
	}
}

/**
 * Counts encoder pulses enough to reach the desired symbol position, then stops.
 */
bool Reel::counting()
{
	if((micros() - lastPulse > ENCODERDEBOUNCE) && (digitalRead(encoderPin) != pulseSignal)) {
		lastPulse = micros();
		pulseSignal = !pulseSignal;
		if(pulseSignal) {  // RISING flank
			if(nSteps-- + HOMEOFFSET == 0) {
				#if DEBUGREELS
				Serial.println("  Reel #" + String(index) + " stopped");
				#endif
				motor.Brake();
				reelState = ReelState::IDLE;
			}
		}
	}
}

// ----------------------------------------------------- Public member functions

void Reel::Setup(const uint8_t reelIndex)
{
	// Sets up variables

	index = reelIndex;
	encoderPin = encoderPins[index];
	homePin = homeSensorPins[index];
	normalSpeed = normalSpeeds[index];
	slowSpeed = slowSpeeds[index];

	// Set pin modes

	pinMode(encoderPin, INPUT_PULLUP);
	pinMode(homePin, INPUT);

	// Sets up objects

	motor.Init(motorOutPins[index]);
}

/**
 * Does the necessary calculations, draws a symbol and starts the reel.
 * @returns Returns the number of additional turns for next reel.
 */
uint8_t Reel::Start(bool home, const uint8_t prevExtraTurns)
{
	if(home) {
		extraTurns = 0;
		symbolPos = 0;

	} else {
		// Sets the amount of initial full turns per reel
		extraTurns = SPEEDUP || CALIBRATE ? 0 : prevExtraTurns;

		// Draws the final position for this wheel
		symbolPos = CALIBRATE ? 0 : TrueRandom.random(NREELSYMBOLS);
	}

	// Calculates the number of steps necessary to reach the end position

	finalSteps = stepOffsets[symbolPos];
	reelState = ReelState::START;

	return home || SPEEDUP || CALIBRATE ? 0 : extraTurns + 1;
}

/**
 * Rotates the motor backwards for a very short time
 */
void Reel::BounceBack()
{
	if(!locks.IsLocked(index)) {
		motor.FullRotateCCW();
		delay(BOUNCETIME);
		motor.Brake();
	}
}

/**
 * Rotates the motor forward for a very short time
 */
void Reel::BounceForward()
{
	if(!locks.IsLocked(index)) {
		motor.FullRotateCW();
		delay(BOUNCETIME);
		motor.Brake();
	}
}

/**
 * Reel state machine. Returns `true` if the reel is spinning.
 */
bool Reel::Loop()
{
	// State machine for this reel

#if SIMULATE

	switch(reelState) {
		case ReelState::IDLE:
			nSimTicks = SIMULATETICKS;
			break;

		case ReelState::START:
			if(nSimTicks--) {
			} else {
				reelState = ReelState::IDLE;
			}
			break;
	}

#else

	switch(reelState) {
		case ReelState::IDLE:
			idle();
			break;

		case ReelState::START:
			start();
			break;

		case ReelState::SENSING:
			sensing();
			break;

		case ReelState::COUNTING:
			counting();
			break;
	}

#endif

	return reelState != ReelState::IDLE;
}

// ------------------------------------------------------------------------- End
