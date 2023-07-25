
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "reel.h"

// --------------------------------------------------------------- Class members

Reel::Reel()
{
};

Reel::Reel(
	const uint8_t motorPins[2],
	const uint8_t encoderPinNumber,
	const uint8_t homeSensorPin,
	const uint8_t lockButtonPin,
	const uint8_t lockLEDPinNumber,
	const uint8_t motorSpeedValue,
	int *reelComposition
)
{
	// Copy some pin number values

	encoderPin = encoderPinNumber;

	lockLEDPin = lockLEDPinNumber;
	pinMode(lockLEDPin, OUTPUT);
	motorSpeed = motorSpeedValue;
	composition = reelComposition;

	motor = MotorDriver(motorPins, encoderPin);
	ezHomeSensor = ezButton(homeSensorPin);
	ezLockBtn = ezButton(lockButtonPin);
	ezLockBtn.setDebounceTime(BTN_DEBOUNCE);
}

/**
 * Does the necessary calculations, draws 3 symbols and starts the reels.
 */
uint8_t Reel::Start(bool home, uint8_t previousExtraTurns)
{
	if(home) {

		// Move reel to home position

		extraTurns = 0;
		symbolPos = 0;

	} else {

		// Sets the amount of initial full turns per reel
#if SPEEDUP || CALIBRATE || SIMULATE
		extraTurns = 0;
#else
		extraTurns = previousExtraTurns + TrueRandom.random(0, 3);
#endif

		// Draws the final position for each wheel
#if CALIBRATE
		symbolPos = 0;
#else
		symbolPos = TrueRandom.random(NREELSYMBOLS);
#endif

	}

	// Calculates the number of steps necessary to reach each position

	finalSteps = stepOffsets[symbolPos];
	reelState = ReelState::START;

	return extraTurns;
}

void Reel::Simulate()
{
	delay(SIMULATE_DELAY);
	reelState = ReelState::IDLE;
}

/**
 * State machine for each reel while spinning.
 */
void Reel::ProcessSpinning()
{
	// if(locked) {
	// 	reelState = ReelState::IDLE;
	// 	return;
	// }

	ezHomeSensor.loop();

	// ezHomeSensor.loop();

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
					counter = finalSteps;
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
					if(counter-- + homeOffset == 0) {
						motor.Brake();
						reelState = ReelState::IDLE;
					}
				}
			}
			break;
	}
}

/**
 * Set reel lock and LED status.
 */
void Reel::ProcessStopped(bool blinkStatus)
{
	ezLockBtn.loop();

	if(lockable) {
		if(ezLockBtn.isPressed()) {
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

/**
 * Force stop the reel.
 */
void Reel::ForceStop()
{
	motor.Coast();
}


// ------------------------------------------------------------------------- End
