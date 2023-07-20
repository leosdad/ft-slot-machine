
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include <Wire.h>
#include <ezButton.h>
#include <TrueRandom.h>
#include <SevenSegDisplay.h>
#include <MotorDriver.h>

#include "SlotsMain.h"
#include "reel.h"

#pragma region ---------------------------------------------------- Initializers

// Motor setup

MotorDriver motor[] = {
	MotorDriver(motor1Out, encoder[0]),
	MotorDriver(motor2Out, encoder[1]),
	MotorDriver(motor3Out, encoder[2])
};

// EzButtons

ezButton startLever(leverButtonPin);
ezButton increaseBet(increaseBetPin);
ezButton decreaseBet(decreaseBetPin);
ezButton posSensor[NREELS] = {positionSensor[0], positionSensor[1], positionSensor[2]};
ezButton reelBtn[NREELS] = {reelButtonPin[0], reelButtonPin[1], reelButtonPin[2]};

#pragma endregion

#pragma region ------------------------------------------------------- Variables

// TODO: join these values in a struct. Should also include posSensor, reelBtn...
// Maybe even lockLED, etc. (see slots.h)?
bool lockReel[NREELS] = {false, false, false};	// State of reel locks
byte currentSignal[NREELS];					// Used for encoder debouncing
ReelStatus reelState[NREELS];				// State machines for the reels
uint16_t counter[NREELS];					// Position counters
uint16_t finalSteps[NREELS] = {0, 0, 0};	// Steps after sensor is triggered
uint16_t pos[NREELS] = {0, 0, 0};			// Position of symbol to be displayed (0-11)
uint8_t extraTurns[NREELS] = {0, 0, 0};		// Number of extra 360° revolutions per wheel
uint8_t rotations[NREELS];
uint8_t speed[NREELS];
unsigned long lastChange[NREELS]; 	 		// In microseconds; Used with encoders

enum class LockableStatus {
	LOCKABLE = 0,
	NOT_LOCKABLE,
	NEXT_LOCABLE
};

typedef struct __Reels
{
	bool lockable;
	bool locked;
} __Reel;

__Reel reel[NREELS];

bool playing = false;		// Game status
bool spinning = false;		// Game status

char displayBuffer[DISPLAYCHARS];			// Used for the 7-segment display
uint16_t nCoins = 3;						// Current number of coins
uint16_t payoff[NPAYLINES] = {0, 0};		// Payoff for each payline
uint16_t spinPayoff = 0;					// Payoff amount for last spin
uint16_t totalSpins = 0;					// Total spins since the beginning
uint16_t totalWins = 0;						// Total wins since the beginning

// LED blinking

const long blinkInterval = BLINKINTERVAL;
unsigned long blinkPreviousMs = 0;
int blinkLedState = LOW;

Reel myReel[3];

#pragma endregion --------------------------------------------------------------

void SlotsMain::Setup()
{
	sevenSegSetup();
	ioSetup();
	debug.Setup();
	od.Setup();

	for(int i = 0; i < NREELS; i++) {
		Reel myReel[i](
			motorOut[i],
			encoder[i],
			positionSensor[i],
			reelButtonPin[i],
			lockLED[i],
			reelSpeed[i],
			reels[i]);
	}

	changeBet(0);
	Display::U2s(displayBuffer, spinPayoff);
	Display::Show(displayBuffer);

	od.SetFont(Font::MONO_BOLD);
	od.PrintS(2, 8, "Coins");

	startReels(true);
}

void SlotsMain::Loop()
{
	// ezButtons loops

	increaseBet.loop();
	decreaseBet.loop();
	startLever.loop();
	reelBtn[0].loop();
	reelBtn[1].loop();
	reelBtn[2].loop();

	if(spinning) {

		processReel(0);
		processReel(1);
		processReel(2);

		if(isIdle()) {
			resetVars();
			debug.DisplayS("Stopped ");
			if(spinPayoff) {
				changeBet((spinPayoff * nCoins) - nCoins);
			} else {
				changeBet(-nCoins);
			}
			Display::U2s(displayBuffer, spinPayoff);
			Display::Show(displayBuffer);
		}

	} else {

		if(startLever.isPressed()) {
			startReels(false);
		} else if(increaseBet.isPressed()) {
			changeBet(1);
		} else if(decreaseBet.isPressed()) {
			changeBet(-1);
		} else {
			for(int i = 0; i < NREELS; i++) {
				if(reel[i].lockable) {
					if(reelBtn[i].isPressed()) {
						reel[i].locked = !reel[i].locked;
					}
					if(reel[i].locked) {
						digitalWrite(lockLED[i], HIGH);
					}
				} else {
					digitalWrite(lockLED[i], LOW);
				}
			}

			// Blink reel lock LEDs

			blinkReelLockLeds();
		}
	}
}

#pragma region ------------------------------------------------- Private methods

void lockUnlock()
{
	for(int i = 0; i < NREELS; i++) {
		if(reel[i].locked) {
			for(int r = 0; r < NREELS; r++) {
				reel[r].lockable = false;
				reel[i].locked = false;
			}
			return;
		}
	}

	for(int i = 0; i < NREELS; i++) {
		reel[i].locked = false;
		reel[i].lockable = true;
	}
}

/**
 * Blink reel lock LEDs
 */
void SlotsMain::blinkReelLockLeds()
{
	unsigned long currMs = millis();

	if(currMs - blinkPreviousMs >= blinkInterval) {
		blinkPreviousMs = currMs;
		blinkLedState = !blinkLedState;
		for(int i = 0; i < NREELS; i++) {
			if(reel[i].lockable) {
				digitalWrite(lockLED[i], blinkLedState);
			}
		}
	}
}

/**
 * Does the necessary calculations, draws 3 symbols and starts the reels.
 */
void SlotsMain::startReels(bool home)
{
	if(home) {

		// Move reels to home position

		extraTurns[0] = extraTurns[1] = extraTurns[2] = 0;
		pos[0] = pos[1] = pos[2] = 0;

	} else {

		// Sets the amount of initial full turns per reel
#if SPEEDUP || CALIBRATE || SIMULATE
		extraTurns[0] = extraTurns[1] = extraTurns[2] = 0;
#else
		extraTurns[0] = TrueRandom.random(0, 3);
		extraTurns[1] = TrueRandom.random(extraTurns[0] + 1, 4);
		extraTurns[2] = TrueRandom.random(extraTurns[1] + 1, 5);
#endif

		// Draws the final position for each wheel
#if CALIBRATE
		pos[0] = pos[1] = pos[2] = 0;
#else
		pos[0] = TrueRandom.random(NREELSYMBOLS);
		pos[1] = TrueRandom.random(NREELSYMBOLS);
		pos[2] = TrueRandom.random(NREELSYMBOLS);
#endif

	}

	// Calculates the number of steps necessary to reach each position

	finalSteps[0] = stepOffsets[pos[0]];
	finalSteps[1] = stepOffsets[pos[1]];
	finalSteps[2] = stepOffsets[pos[2]];

	// Calculates the payoff for all paylines

	if(playing) {
		uint16_t total = 0;
		for(int l = 0; l < NPAYLINES; l++) {
			total += payoff[l] = payoffs.Calculate(l, pos);
		}
		if(total) {
			totalWins++;
		}
		spinPayoff += total;
		totalSpins++;
	}

	debug.ShowReelPreview(totalSpins, totalWins, extraTurns, payoff, pos);

	// Display::U2s(displayBuffer, spinPayoff);
	// Display::Show(displayBuffer);

	// Starts the wheels

	playing = true;
	debug.DisplayS("Spinning");
	spinning = true;

	// TODO: should remove from here

	lockUnlock();
	prepareNextSpin(ReelStatus::START);
}

/**
 * State machine for each reel.
 */
void SlotsMain::processReel(int nReel)
{
#if SIMULATE
	delay(100);
	state[nReel] = IDLE;
#else
	if(lockReel[nReel]) {
		reelState[nReel] = ReelStatus::IDLE;
		return;
	}
	posSensor[nReel].loop();

	switch(reelState[nReel]) {

		case ReelStatus::START:
			rotations[nReel] = extraTurns[nReel];
			speed[nReel] = reelSpeed[nReel];
			motor[nReel].RotateCW(speed[nReel]);
			reelState[nReel] = ReelStatus::SENSING;
			break;

		case ReelStatus::SENSING:
			if(posSensor[nReel].isReleased()) {
				if(rotations[nReel] > 0) {
					rotations[nReel]--;
				} else {
					counter[nReel] = finalSteps[nReel];
					currentSignal[nReel] = digitalRead(encoder[nReel]);
					reelState[nReel] = ReelStatus::COUNTING;
				}
			}
			break;

		case ReelStatus::COUNTING:
			if(((micros() - lastChange[nReel]) > debouncePeriod) &&
				(digitalRead(encoder[nReel]) != currentSignal[nReel])) {
				lastChange[nReel] = micros();
				currentSignal[nReel] = !currentSignal[nReel];
				if(currentSignal[nReel]) {	// RISING flank
					if(counter[nReel]-- + homeOffset == 0) {
						motor[nReel].Brake();
						reelState[nReel] = ReelStatus::IDLE;
					}
				}
			}
			break;
	}
#endif
}

/**
 * Returns true if all reels are in idle state.
 */
bool SlotsMain::isIdle()
{
	for(int i = 0; i < NREELS; i++) {
		if(reelState[i] != ReelStatus::IDLE) {
			return false;
		}
	}
	return true;
}

/**
 * Turns off state variables and put machine to idle state.
 */
void SlotsMain::resetVars()
{
	spinning = false;
	for(int i = 0; i < NREELS; i++) {
		reelState[i] = ReelStatus::START;
	}
	prepareNextSpin(ReelStatus::IDLE);
	for(int i = 0; i < NREELS; i++) {
		lockReel[i] = false;
		digitalWrite(lockLED[i], LOW);
	}
}

/**
 * Resets the three reels prior to the next spin.
 */
void SlotsMain::prepareNextSpin(ReelStatus newState)
{
	for(int i = 0; i < NREELS; i++) {
		MotorDriver(motor1Out, encoder[0]), counter[i] = 0;
		currentSignal[i] = 0;
		lastChange[i] = 0;
		reelState[i] = newState;
		speed[i] = reelSpeed[i];
		rotations[i] = 0;
	}
}

/**
 * Increments the number of coins by the amount given.
 */
void SlotsMain::changeBet(uint16_t bet)
{
	nCoins = min(maxCoins, max(0, (signed)(nCoins + bet)));
	od.SetFont(Font::DIGITS_EXTRALARGE);
	od.PrintN(1, 3, nCoins);
}

/**
 * Initializes the seven-segment display.
 */
void SlotsMain::sevenSegSetup()
{
	// Reset seven-segment display

	Wire.begin();
	Display::Init();
	Display::Clear();
	Display::Stop();
}

/**
 * Sets initial pin modes for the Arduino.
 */
void SlotsMain::ioSetup()
{
	startLever.setDebounceTime(50);
	increaseBet.setDebounceTime(50);
	decreaseBet.setDebounceTime(50);
	reelBtn[0].setDebounceTime(50);
	reelBtn[1].setDebounceTime(50);
	reelBtn[2].setDebounceTime(50);

	pinMode(redLED1[0], OUTPUT);
	pinMode(redLED1[0], OUTPUT);
	pinMode(redLED2[1], OUTPUT);
	pinMode(redLED2[1], OUTPUT);
	pinMode(lockLED[0], OUTPUT);
	pinMode(lockLED[1], OUTPUT);
	pinMode(lockLED[2], OUTPUT);
}

/**
 * Force stop all three reels.
 */
void SlotsMain::forceStopReels()
{
	for(int i = 0; i < NREELS; i++) {
		motor[i].Coast();
	}
}

#pragma endregion
