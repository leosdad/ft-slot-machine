
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include <ezButton.h>
#include <ezLED.h>
#include <TrueRandom.h>

#include "slots.h"
#include "reel.h"

// ---------------------------------------------------- Private member functions

bool Reel::idle()
{
	// Does nothing
}

bool Reel::start()
{
	rotations = extraTurns;
	motor.RotateCW(motorSpeed);
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

void Reel::Setup(
	const uint8_t motorOutPinNumbers[2],
	const uint8_t encoderPinNumber,
	const uint8_t homeSensorPinNumber,
	const uint8_t motorSpeedValue
)
{
	// Setup variables

	encoderPin = encoderPinNumber;
	homePin = homeSensorPinNumber;
	motorSpeed = motorSpeedValue;

	// Set pin modes

	pinMode(encoderPin, INPUT_PULLUP);
	pinMode(homePin, INPUT);
	pinMode(motorOutPinNumbers[0], OUTPUT);
	pinMode(motorOutPinNumbers[1], OUTPUT);

	// Setup objects

	encoderPin = encoderPinNumber;
	homePin = homeSensorPinNumber;
	motorSpeed = motorSpeedValue;
	motor = MotorDriver(motorOutPinNumbers);
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
 * State machine. Returns `true` if the reel is spinning.
 */
bool Reel::Loop()
{
	if(SIMULATE) {
		delay(SIMULATEDELAY);
		reelState = ReelState::IDLE;
		return;
	}

	// State machine for this reel

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

	return reelState != ReelState::IDLE;
}

// ------------------------------------------------------------------------- End
