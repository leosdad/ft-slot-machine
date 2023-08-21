
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "reel.h"

// ---------------------------------------------------- Private member functions

bool Reel::idle()
{
	bool changed = false;

	if(lockable) {
		if(ezLockButton.isPressed()) {
			locked = !locked;
			changed = true;
		}

		if(locked != lastLockedValue) {
			if(locked) {
				ezLockLED.turnON();
			} else {
				ezLockLED.blink(2, 200, 0);
			}
			lastLockedValue = locked;
		}

	} else {
		ezLockLED.turnOFF();
	}

	return changed;
}

bool Reel::start()
{
	rotations = extraTurns;
	motor.RotateCW(motorSpeed);
	reelState = ReelState::SENSING;
}

/**
 * Decrements rotation counter and waits for the reel to reach the home position.
 */
bool Reel::sensing()
{
	if(ezHomeSensor.isReleased()) {
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
 * Counts pulses enough to reach the desired symbol, then stops.
 */
bool Reel::counting()
{
	// TODO: reduce speed
	if(((micros() - lastChange) > debouncePeriod) && (digitalRead(encoderPin) != currentSignal)) {
		lastChange = micros();
		currentSignal = !currentSignal;
		if(currentSignal) {	// RISING flank
			if(nSteps-- + homeOffset == 0) {
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
	const uint8_t lockButtonPinNumber,
	const uint8_t lockLEDPinNumber,
	const uint8_t motorSpeedValue
)
{
	// Setup objects

	encoderPin = encoderPinNumber;
	motorSpeed = motorSpeedValue;
	motor = MotorDriver(motorOutPinNumbers, encoderPin);

	// Initialize ezButtons

	ezHomeSensor = ezButton(homeSensorPinNumber);
	ezLockButton = ezButton(lockButtonPinNumber);
	ezLockButton.setDebounceTime(EZBTNDEBOUNCE);
	ezLockLED = ezLED(lockLEDPinNumber);

	// Initialize variables

	reelState = ReelState::IDLE;
}

/**
 * Does the necessary calculations, draws a symbol and starts the reel.
 * @returns Returns the number of additional turns for this reel.
 */
uint8_t Reel::Start(bool home, uint8_t previousExtraTurns)
{
	// If reel is locked, Does nothing

	// if(locked) {
	// 	return;
	// }

	if(home) {

		// Move reel to home position

		extraTurns = 0;
		symbolPos = 0;

	} else {

		// Sets the amount of initial full turns per reel
		extraTurns = speedUp || calibrate ? 0 :
			previousExtraTurns + TrueRandom.random(0, 3);

		// Draws the final position for this wheel
		symbolPos = calibrate ? 0 : TrueRandom.random(NREELSYMBOLS);
	}

	// Calculates the number of steps necessary to reach the end position

	finalSteps = stepOffsets[symbolPos];
	reelState = ReelState::START;

	if(locked) {
		ezLockLED.turnON();
	} else {
		ezLockLED.turnOFF();
	}

	lastLockedValue = -1;

	return extraTurns;
}

/**
 * State machine.
 */
void Reel::Loop()
{
	if(simulate) {
		delay(SIMULATEDELAY);
		reelState = ReelState::IDLE;
		return;
	}

	ezHomeSensor.loop();
	ezLockButton.loop();
	ezLockLED.loop();

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
}

/**
 * Returns true if the reel is in idle state.
 */
bool Reel::IsIdle()
{
	return reelState == ReelState::IDLE;
}

// ------------------------------------------------------------------------- End
