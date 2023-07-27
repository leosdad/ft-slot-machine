
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include <ezButton.h>
#include <TrueRandom.h>

#include "SlotsMain.h"
#include "seven-seg.h"
#include "motor-driver.h"
#include "game.h"
#include "spin.h"
#include "reel.h"

#pragma region ---------------------------------------------------- Initializers

// EzButtons

ezButton startLever(leverButtonPin);
ezButton increaseBet(increaseBetPin);
ezButton decreaseBet(decreaseBetPin);

// Slot machine objects

Reel myReel[NREELS];
Game game;
SevenSeg sevenSegDisplay;
OledShow od;

#pragma endregion

#pragma region ------------------------------------------------- Other variables

uint16_t payoff[NPAYLINES] = {0, 0};	// Payoff for each payline
uint16_t spinPayoff = 0;				// Payoff amount for last spin

// LED blinking

const long blinkInterval = BLINKINTERVAL;
unsigned long blinkPreviousMs = 0;
int blinkLedState = LOW;

#pragma endregion --------------------------------------------------------------

void SlotsMain::Setup()
{
	// Initialize objects

	ioSetup();
	cmdReels(ReelCommands::INIT);
	od.Setup(DEBUGINFO);
	sevenSegDisplay.Setup();

	// Reset reels to home posiion and start game with initial bet

	sevenSegDisplay.DisplayNumber(spinPayoff);
	startSpinning(true);
	od.DisplayBet(game.SetBet(STARTCOINS));
}

void SlotsMain::Loop()
{
	// ezButtons loops

	increaseBet.loop();
	decreaseBet.loop();
	startLever.loop();

	if(game.spinning) {

		cmdReels(ReelCommands::PROCESSSPINNING);

		if(isIdle()) {
			game.spinning = false;
			cmdReels(ReelCommands::RESET);
			od.ShowState("Stopped ");
			if(spinPayoff) {
				game.ChangeBet((spinPayoff * game.nCoins) - game.nCoins);
			} else {
				game.ChangeBet(-game.nCoins);
			}
			od.DisplayBet(game.nCoins);
			sevenSegDisplay.DisplayNumber(spinPayoff);
		}

	} else {

		if(startLever.isPressed()) {
			startSpinning(false);
		} else if(increaseBet.isPressed()) {
			game.ChangeBet(1);
			od.DisplayBet(game.nCoins);
		} else if(decreaseBet.isPressed()) {
			game.ChangeBet(-1);
			od.DisplayBet(game.nCoins);
		} else {
			cmdReels(ReelCommands::PROCESSSTOPPED);
			blinkLedsTimer();
		}
	}
	
	// digitalWrite(signalLED1[1], spinning);
	// digitalWrite(signalLED2[1], spinning);
}

#pragma region ------------------------------------------------- Private methods

void SlotsMain::startSpinning(bool home)
{
	if(home) {
		game.SetBet(0);
	}
	cmdReels(home ? ReelCommands::STARTHOME : ReelCommands::START);
	calculateAllPayoffs();
	od.ShowReelPreview(game, myReel, payoff);
	game.playing = true;
	game.spinning = true;
	od.ShowState("Spinning");
}

/**
 * Calculates the payoff for all paylines
 */
void SlotsMain::calculateAllPayoffs()
{
	if(game.playing) {
		uint16_t total = 0;
		for(int l = 0; l < NPAYLINES; l++) {
			total += payoff[l] = payoffs.Calculate(l, myReel);
		}
		if(total) {
			game.totalWins++;
		}
		spinPayoff += total;
		game.totalSpins++;
	}
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
 * Receives a command and performs the respective actions on all reels.
*/
void SlotsMain::cmdReels(ReelCommands cmd)
{
	for(int i = 0; i < NREELS; i++) {
		switch(cmd) {
			case ReelCommands::INIT:
				myReel[i] = Reel(
					motorOut[i],
					encoderPin[i],
					homeSensorPin[i],
					lockButtonPin[i],
					lockLEDPin[i],
					motorSpeed[i],
					(int *)reelComposition[i]
				);
				break;

			case ReelCommands::START:
				myReel[i].Start(false, 0);
				break;

			case ReelCommands::STARTHOME:
				myReel[i].Start(true, 0);
				break;

			case ReelCommands::RESET:
				myReel[i].Reset(false);
				break;

			case ReelCommands::RESETWITHSTART:
				myReel[i].Reset(true);
				break;

			case ReelCommands::PROCESSSPINNING:
				myReel[i].ProcessSpinning();
				break;

			case ReelCommands::PROCESSSTOPPED:
				myReel[i].ProcessStopped(blinkLedState);
				break;
		}
	}
}

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

/**
 * Sets initial pin modes for the Mega 2560.
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

#pragma endregion
