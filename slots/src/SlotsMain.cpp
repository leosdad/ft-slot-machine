
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include <Wire.h>
#include <ezButton.h>
#include <TrueRandom.h>
#include <SevenSegDisplay.h>

#include "SlotsMain.h"
#include "motor-driver.h"
#include "reel.h"

#pragma region ---------------------------------------------------- Initializers

// EzButtons

ezButton startLever(leverButtonPin);
ezButton increaseBet(increaseBetPin);
ezButton decreaseBet(decreaseBetPin);
// ezButton posSensor[NREELS] = {homeSensorPin[0], homeSensorPin[1], homeSensorPin[2]};
// ezButton reelBtn[NREELS] = {lockButtonPin[0], lockButtonPin[1], lockButtonPin[2]};

#pragma endregion

#pragma region ------------------------------------------------------- Variables

// TODO: join these values in a struct. Should also include posSensor, reelBtn...
// Maybe even lockLED, etc. (see slots.h)?
// bool lockReel[NREELS] = {false, false, false};	// State of reel locks
// byte currentSignal[NREELS];					// Used for encoder debouncing
// ReelStatus reelState[NREELS];				// State machines for the reels
// uint16_t counter[NREELS];					// Position counters
// uint16_t finalSteps[NREELS] = {0, 0, 0};	// Steps after sensor is triggered
// uint16_t pos[NREELS] = {0, 0, 0};			// Position of symbol to be displayed (0-11)
// uint8_t extraTurns[NREELS] = {0, 0, 0};		// Number of extra 360° revolutions per wheel
// uint8_t rotations[NREELS];
// uint8_t speed[NREELS];
// unsigned long lastChange[NREELS]; 	 		// In microseconds; Used with encoders

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

// Reel myReel[3];
// ezButton reelBtn1 = lockButtonPin[0];
Reel myReel[NREELS];//{{0,0},{0,0},{0,0}};

//= Reel(reelBtn1);

#pragma endregion --------------------------------------------------------------

void SlotsMain::Setup()
{
	sevenSegSetup();
	ioSetup();
	debug.Setup();
	od.Setup();

	for(int i = 0; i < NREELS; i++) {
		myReel[i] = Reel(
			motorOut[i],
			encoderPin[i],
			homeSensorPin[i],
			lockButtonPin[i],
			lockLEDPin[i],
			motorSpeed[i],
			(int *)reelComposition[i]
		);
	}

	changeBet(0);
	Display::U2s(displayBuffer, spinPayoff);
	Display::Show(displayBuffer);

#if not defined SHOWDEBUGDATA
	od.SetFont(Font::MONO_BOLD);
	od.PrintS(2, 8, "Coins");
#endif

	for(int i = 0; i < NREELS; i++) {
		myReel[i].Start(true, 0);
	}

	__unnamed();
}

void SlotsMain::Loop()
{
	// ezButtons loops

	increaseBet.loop();
	decreaseBet.loop();
	startLever.loop();
	// reelBtn[0].loop();
	// reelBtn[1].loop();
	// reelBtn[2].loop();

	if(spinning) {

		for(int i = 0; i < NREELS; i++) {
			myReel[i].Process();
		}
		// processReel(0);
		// processReel(1);
		// processReel(2);

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
			// startReels(false);
			for(int i = 0; i < NREELS; i++) {
				myReel[i].Start(false, 0);
			}
			__unnamed();
		} else if(increaseBet.isPressed()) {
			changeBet(1);
		} else if(decreaseBet.isPressed()) {
			changeBet(-1);
		} else {
			// for(int i = 0; i < NREELS; i++) {
			// 	if(reel[i].lockable) {
			// 		if(reelBtn[i].isPressed()) {
			// 			reel[i].locked = !reel[i].locked;
			// 		}
			// 		if(reel[i].locked) {
			// 			digitalWrite(lockLED[i], HIGH);
			// 		}
			// 	} else {
			// 		digitalWrite(lockLED[i], LOW);
			// 	}
			// }

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
				// digitalWrite(lockLED[i], blinkLedState);
			}
		}
	}
}

void SlotsMain::__unnamed()
{
	// Calculates the payoff for all paylines

	if(playing) {
		uint16_t total = 0;
		for(int l = 0; l < NPAYLINES; l++) {
			// total += payoff[l] = payoffs.Calculate(l, pos);
		}
		if(total) {
			totalWins++;
		}
		spinPayoff += total;
		totalSpins++;
	}

	// debug.ShowReelPreview(totalSpins, totalWins, extraTurns, payoff, pos);

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
 * Returns true if all reels are in idle state.
 */
bool SlotsMain::isIdle()
{
	for(int i = 0; i < NREELS; i++) {
		if(!myReel[i].IsIdle()) {
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
	// for(int i = 0; i < NREELS; i++) {
	// 	reelState[i] = ReelStatus::START;
	// }
	// prepareNextSpin(ReelStatus::IDLE);
	// for(int i = 0; i < NREELS; i++) {
	// 	lockReel[i] = false;
	// 	digitalWrite(lockLED[i], LOW);
	// }
}

/**
 * Resets the three reels prior to the next spin.
 */
void SlotsMain::prepareNextSpin(ReelStatus newState)
{
	for(int i = 0; i < NREELS; i++) {
		// MotorDriver(motor1Out, encoder[0]), counter[i] = 0;
		// currentSignal[i] = 0;
		// lastChange[i] = 0;
		// reelState[i] = newState;
		// speed[i] = reelSpeed[i];
		// rotations[i] = 0;
	}
}

/**
 * Increments the number of coins by the amount given.
 */
void SlotsMain::changeBet(uint16_t bet)
{
	nCoins = min(maxCoins, max(0, (signed)(nCoins + bet)));
#if not defined SHOWDEBUGDATA	
	od.SetFont(Font::DIGITS_EXTRALARGE);
	od.PrintN(1, 3, nCoins);
#endif
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
	// reelBtn[0].setDebounceTime(50);
	// reelBtn[1].setDebounceTime(50);
	// reelBtn[2].setDebounceTime(50);

	pinMode(signalLED1[0], OUTPUT);
	pinMode(signalLED1[0], OUTPUT);
	pinMode(signalLED2[1], OUTPUT);
	pinMode(signalLED2[1], OUTPUT);
	// pinMode(lockLED[0], OUTPUT);
	// pinMode(lockLED[1], OUTPUT);
	// pinMode(lockLED[2], OUTPUT);
}

/**
 * Force stop all three reels.
 */
void SlotsMain::forceStopReels()
{
	for(int i = 0; i < NREELS; i++) {
		// motor[i].Coast();
		myReel[i].ForceStop();
	}
}

#pragma endregion
