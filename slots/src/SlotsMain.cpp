
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include <Wire.h>
#include <ezButton.h>
#include <TrueRandom.h>
#include <SevenSegDisplay.h>

#include "SlotsMain.h"
#include "motor-driver.h"
#include "game.h"
#include "spin.h"
#include "reel.h"

#pragma region ---------------------------------------------------- Initializers

// EzButtons

ezButton startLever(leverButtonPin);
ezButton increaseBet(increaseBetPin);
ezButton decreaseBet(decreaseBetPin);

#pragma endregion

#pragma region ------------------------------------------------------- Variables

// Set to true to show debug data on the OLED display
bool displayDebugInfo = false;

char displayBuffer[DISPLAYCHARS];			// Used for the 7-segment display

// Game

// bool playing = false;						// Game status
// bool spinning = false;						// Game status
// uint16_t nCoins = 3;						// Current number of coins
// uint16_t totalSpins = 0;					// Total spins since the beginning
// uint16_t totalWins = 0;						// Total wins since the beginning

// Spin

uint16_t payoff[NPAYLINES] = {0, 0};		// Payoff for each payline
uint16_t spinPayoff = 0;					// Payoff amount for last spin

// LED blinking

const long blinkInterval = BLINKINTERVAL;
unsigned long blinkPreviousMs = 0;
int blinkLedState = LOW;

Reel myReel[NREELS];
Game game;

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

	if(displayDebugInfo) {
		od.PrintN(1, 1, lockButtonPin[0]);
		od.PrintN(2, 1, lockButtonPin[1]);
	}

	game.changeBet(0);
	displayBet();

	Display::U2s(displayBuffer, spinPayoff);
	Display::Show(displayBuffer);

	if(!displayDebugInfo) {
		od.SetFont(Font::MONO_BOLD);
		od.PrintS(2, 8, "Coins");
	}

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

	if(game.spinning) {

		for(int i = 0; i < NREELS; i++) {
			myReel[i].ProcessSpinning();
		}

		if(isIdle()) {
			resetVars();
			if(!displayDebugInfo) {
				debug.DisplayS("Stopped ");
			}
			if(spinPayoff) {
				game.changeBet((spinPayoff * game.nCoins) - game.nCoins);
			} else {
				game.changeBet(-game.nCoins);
			}
			displayBet();
			Display::U2s(displayBuffer, spinPayoff);
			Display::Show(displayBuffer);
		}

	} else {

		if(startLever.isPressed()) {
			for(int i = 0; i < NREELS; i++) {
				myReel[i].Start(false, 0);
			}
			__unnamed();
		} else if(increaseBet.isPressed()) {
			game.changeBet(1);
			displayBet();
		} else if(decreaseBet.isPressed()) {
			game.changeBet(-1);
			displayBet();
		} else {
			for(int i = 0; i < NREELS; i++) {
				myReel[i].ProcessStopped(blinkLedState);
			}
			blinkLedsTimer();
		}
	}
	
	// digitalWrite(signalLED1[1], spinning);
	// digitalWrite(signalLED2[1], spinning);
}

#pragma region ------------------------------------------------- Private methods

/**
 * Timer for blinking reel lock LEDs
 */
void SlotsMain::blinkLedsTimer()
{
	unsigned long currMs = millis();

	if(currMs - blinkPreviousMs >= blinkInterval) {
		blinkPreviousMs = currMs;
		blinkLedState = !blinkLedState;
	}
}

void SlotsMain::__unnamed()
{
	// Calculates the payoff for all paylines

	if(game.playing) {
		uint16_t total = 0;
		for(int l = 0; l < NPAYLINES; l++) {
			// total += payoff[l] = payoffs.Calculate(l, pos);
		}
		if(total) {
			game.totalWins++;
		}
		spinPayoff += total;
		game.totalSpins++;
	}

	// debug.ShowReelPreview(totalSpins, totalWins, extraTurns, payoff, pos);

	Display::U2s(displayBuffer, spinPayoff);
	Display::Show(displayBuffer);

	// Starts the wheels

	game.playing = true;
	if(!displayDebugInfo) {
		debug.DisplayS("Spinning");
	}
	game.spinning = true;

	// TODO: should remove from here

	// lockAndUnlock();
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
	game.spinning = false;
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
 * Shows the current bet amount on the OLED display.
 */
void SlotsMain::displayBet()
{
	if(!displayDebugInfo) {
		od.SetFont(Font::DIGITS_EXTRALARGE);
		od.PrintN(1, 3, game.nCoins);
	}
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

	pinMode(signalLED1[0], OUTPUT);
	pinMode(signalLED1[0], OUTPUT);
	pinMode(signalLED2[1], OUTPUT);
	pinMode(signalLED2[1], OUTPUT);
}

/**
 * Force stop all three reels.
 */
void SlotsMain::forceStopReels()
{
	for(int i = 0; i < NREELS; i++) {
		myReel[i].ForceStop();
	}
}

#pragma endregion
