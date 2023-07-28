
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include <ezButton.h>
#include <TrueRandom.h>

#include "slots-main.h"
#include "seven-seg.h"
#include "motor-driver.h"
#include "game.h"
#include "reel.h"

#pragma region ------------------------------------------------ Global variables

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

unsigned long blinkPreviousMs = 0;
const long blinkInterval = BLINKINTERVAL;
int blinkLedState = LOW;

#pragma endregion ----------------------------------------------- Setup and Loop

void SlotsMain::Setup()
{
	// Initialize objects

	ioSetup();
	game.Setup(reels);
	game.InitReels(motorOut, encoderPin, homeSensorPin, lockButtonPin,
		lockLEDPin, motorSpeed, (int **)reelComposition);
	od.Setup(DEBUGINFO);
	sevenSegDisplay.Setup();

	// Reset reels to home position and start game with initial values

	sevenSegDisplay.DisplayNumber(0);
	od.DisplayCoins(game.nCoins = STARTCOINS);
	od.DisplayBet(game.SetBet(INITIALBET));
	startGame(true);
}

void SlotsMain::Loop()
{
	inputLoop();

	if(game.spinning) {
		loopWhenSpinning();
	} else {
		loopWhenStopped();
	}
}

#pragma region ---------------------------------------- Private member functions

void SlotsMain::loopWhenSpinning()
{
	game.ProcessWhenSpinning();
	if(game.IsIdle()) {
		stopGame();
	}
}

void SlotsMain::loopWhenStopped()
{
	game.ProcessWhenStopped(blinkLedState);
	blinkLedsTimer();

	if(startLever.isPressed()) {
		if(game.currentBet) {
			startGame(false);
		}
	} else if(increaseBet.isPressed()) {
		od.DisplayBet(game.ChangeBet(1));
	} else if(decreaseBet.isPressed()) {
		od.DisplayBet(game.ChangeBet(-1));
	}
}

void SlotsMain::inputLoop()
{
	// ezButtons loops
	increaseBet.loop();
	decreaseBet.loop();
	startLever.loop();
}

void SlotsMain::startGame(bool home)
{
	game.StartReels(home);
	if(!home) {
		payoffs.CalculateTotalPayoff(&game);
		game.playing = true;
	}
	game.totalSpins++;
	game.spinning = true;
	if(game.playing) {
		game.nCoins -= game.currentBet;
		od.DisplayCoins(game.nCoins);
	}
	od.DisplayDebugInfo(game);
	od.ShowState("Spinning");
}

void SlotsMain::stopGame()
{
	game.spinning = false;
	game.StartReels(false);
	od.ShowState("Stopped ");
	if(game.playing) {
		game.nCoins += game.spinPayoff * game.currentBet;
	}
	od.DisplayCoins(game.nCoins);
	sevenSegDisplay.DisplayNumber(game.spinPayoff);
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
