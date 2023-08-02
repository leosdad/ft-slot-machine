
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "reel.h"

// --------------------------------------------------------------- Class members

void Reel::Setup(
	const uint8_t motorOutPinNumbers[2],
	const uint8_t encoderPinNumber,
	const uint8_t homeSensorPinNumber,
	const uint8_t lockButtonPinNumber,
	const uint8_t lockLEDPinNumber,
	const uint8_t motorSpeedValue
)
{
	encoderPin = encoderPinNumber;
	motorSpeed = motorSpeedValue;
	lockLEDPin = lockLEDPinNumber;
	pinMode(lockLEDPin, OUTPUT);

	motor = MotorDriver(motorOutPinNumbers, encoderPin);
	ezHomeSensor = ezButton(homeSensorPinNumber);
	ezLockButton = ezButton(lockButtonPinNumber);
	ezLockButton.setDebounceTime(EZBTNDEBOUNCE);
}

/**
 * Does the necessary calculations, draws a symbol and starts the reel.
 */
uint8_t Reel::Start(bool home, uint8_t previousExtraTurns)
{
	digitalWrite(lockLEDPin, locked);

	// If reel is locked, Does nothing

	if(locked) {
		return;
	}

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

	return extraTurns;
}

/**
 * Loop called while spinning and state machine for this reel.
 */
void Reel::LoopWhenSpinning()
{
	ezHomeSensor.loop();

	if(simulate) {
		delay(SIMULATEDELAY);
		reelState = ReelState::IDLE;
		return;
	}

	// State machine for this reel

	switch(reelState) {

		case ReelState::START:

			rotations = extraTurns;
			motor.RotateCW(motorSpeed);
			reelState = ReelState::SENSING;
			break;

		case ReelState::SENSING:

			if(ezHomeSensor.isReleased()) {
				if(rotations > 0) {
					rotations--;
				} else {
					nSteps = finalSteps;
					currentSignal = digitalRead(encoderPin);
					reelState = ReelState::COUNTING;
				}
			}
			break;

		case ReelState::COUNTING:

			if(((micros() - lastChange) > debouncePeriod) &&
				(digitalRead(encoderPin) != currentSignal)) {
				lastChange = micros();
				currentSignal = !currentSignal;
				if(currentSignal) {	// RISING flank
					if(nSteps-- + homeOffset == 0) {
						motor.Brake();
						reelState = ReelState::IDLE;
					}
				}
			}
			break;
	}
}

/**
 * Loop called when stopped. Sets reel lock and LED status.
 */
void Reel::LoopWhenStopped(bool blinkStatus)
{
	ezLockButton.loop();

	if(lockable) {
		if(ezLockButton.isPressed()) {
			locked = !locked;
		}
		if(locked) {
			digitalWrite(lockLEDPin, HIGH);
		} else {
			digitalWrite(lockLEDPin, blinkStatus);
		}
	} else {
		digitalWrite(lockLEDPin, LOW);
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
