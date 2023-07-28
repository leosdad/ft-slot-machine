
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

Reel reels[NREELS];
Game game;

// Other objects

SevenSeg sevenSegDisplay;
OledShow od;

// LED blinking

const long blinkInterval = BLINKINTERVAL;
unsigned long blinkPreviousMs = 0;
int blinkLedState = LOW;

#pragma endregion --------------------------------------------------------------

void SlotsMain::Setup()
{
	// Initialize objects

	ioSetup();
	game.Setup(reels);
	cmdReels(ReelCommands::INIT);
	od.Setup(DEBUGINFO);
	sevenSegDisplay.Setup();

	// Reset reels to home posiion and start game with initial bet

	sevenSegDisplay.DisplayNumber(game.spinPayoff);
	startSpinning(true);
	od.DisplayBigNumber(game.SetBet(STARTCOINS));
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
			if(game.spinPayoff) {
				game.ChangeBet((game.spinPayoff * game.nCoins) - game.nCoins);
			} else {
				game.ChangeBet(-game.nCoins);
			}
			sevenSegDisplay.DisplayNumber(game.spinPayoff);
		}

	} else {

		if(startLever.isPressed()) {
			startSpinning(false);
		} else if(increaseBet.isPressed()) {
			od.DisplayBigNumber(game.ChangeBet(1));
		} else if(decreaseBet.isPressed()) {
			od.DisplayBigNumber(game.ChangeBet(-1));
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
	cmdReels(home ? ReelCommands::STARTHOME : ReelCommands::START);
	calculateAllPayoffs();
	od.ShowReelPreview(game);
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
			total += game.payoff[l] = payoffs.Calculate(l, game.reels);
		}
		if(total) {
			game.totalWins++;
		}
		game.spinPayoff += total;
		game.totalSpins++;
	}
}

/**
 * Returns true if all reels are in idle state.
 */
bool SlotsMain::isIdle()
{
	for(int i = 0; i < NREELS; i++) {
		if(!game.reels[i].IsIdle()) {
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
				game.reels[i] = Reel(
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
				game.reels[i].Start(false, 0);
				break;

			case ReelCommands::STARTHOME:
				game.reels[i].Start(true, 0);
				break;

			case ReelCommands::RESET:
				game.reels[i].Reset(false);
				break;

			case ReelCommands::RESETWITHSTART:
				game.reels[i].Reset(true);
				break;

			case ReelCommands::PROCESSSPINNING:
				game.reels[i].ProcessSpinning();
				break;

			case ReelCommands::PROCESSSTOPPED:
				game.reels[i].ProcessStopped(blinkLedState);
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
