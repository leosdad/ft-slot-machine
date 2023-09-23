
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include <ezButton.h>
#include <TrueRandom.h>

#include "slots.h"
#include "reel.h"
#include "locks.h"

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
	motor.RotateCW(normalSpeed);
	reelState = ReelState::SENSING;
}

/**
 * Decrements rotation counter and waits until the reel reaches the home position.
 */
bool Reel::sensing()
{
	if(digitalRead(homePin)) {
		if(rotations > 0) {
			rotations--;
		} else {
			nSteps = finalSteps;
			currentSignal = digitalRead(encoderPin);
			motor.RotateCW(slowSpeed);
			reelState = ReelState::COUNTING;
		}
	}
}

/**
 * Counts encoder pulses enough to reach the desired symbol position, then stops.
 */
bool Reel::counting()
{
	if(((micros() - lastChange) > ENCODERDEBOUNCE) && (digitalRead(encoderPin) != currentSignal)) {
		lastChange = micros();
		currentSignal = !currentSignal;
		if(currentSignal) {	// RISING flank
			if(nSteps-- + HOMEOFFSET == 0) {
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
 * @returns Returns the number of additional turns for this reel.
 */
uint8_t Reel::Start(bool home, uint8_t previousExtraTurns)
{
	if(home) {

		// Move reel to home position

		extraTurns = 0;
		symbolPos = 0;

	} else {

		// Sets the amount of initial full turns per reel
		extraTurns = SPEEDUP || CALIBRATE ? 0 :
			previousExtraTurns + TrueRandom.random(0, 3);

		// Draws the final position for this wheel
		symbolPos = CALIBRATE ? 0 : TrueRandom.random(NREELSYMBOLS);
	}

	// Calculates the number of steps necessary to reach the end position

	finalSteps = stepOffsets[symbolPos];
	reelState = ReelState::START;

	return extraTurns;
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
