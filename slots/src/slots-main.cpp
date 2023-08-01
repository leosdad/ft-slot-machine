
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
OledDisplay od;

#pragma endregion ----------------------------------------------- Setup and Loop

void SlotsMain::Setup()
{
	// Initialize objects

	ioSetup();
	game.Setup(reels);
	game.InitReels(motorOutPins, encoderPins, homeSensorPins,
		lockButtonPins, lockLEDPins, motorSpeed);
	od.Setup(DEBUGINFO);
	sevenSegDisplay.Setup();

	// Reset reels to home position and start game with initial values

	sevenSegDisplay.DisplayNumber(0);
	od.DisplayCoins(game.nCoins = STARTCOINS);
	od.DisplayBet(game.SetBet(INITIALBET));
	startSpin(true);
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

// TODO: Move some code from here to the Game class

void SlotsMain::loopWhenSpinning()
{
	game.LoopWhenSpinning();
	if(game.IsIdle()) {
		stopSpin();
	}
}

void SlotsMain::loopWhenStopped()
{
	game.LoopWhenStopped();

	if(startLever.isPressed()) {
		if(game.currentBet) {
			startSpin(false);
		}
	} else if(increaseBet.isPressed()) {
		od.DisplayBet(game.ChangeBet(1));
	} else if(decreaseBet.isPressed()) {
		od.DisplayBet(game.ChangeBet(-1));
	}
}

void SlotsMain::startSpin(bool home)
{
	// TODO: move to Game class

	game.StartReels(home);
	if(!home) {
		payoffs.CalculateTotalPayoff(&game);
		game.playing = true;
	}
	game.totalSpins++;
	game.spinning = true;
	if(game.playing) {
		game.nCoins = constrain(game.nCoins - game.currentBet, 0, MAXCOINS);
		od.DisplayCoins(game.nCoins);
	}
	od.DisplayDebugInfo(game);
	od.ShowState("Spinning");
}

void SlotsMain::stopSpin()
{
	game.StopSpin();
	od.DisplayBet(game.SetBet(game.currentBet));
	od.ShowState("Stopped ");
	od.DisplayCoins(game.nCoins);
	sevenSegDisplay.DisplayNumber(game.spinPayoff * game.currentBet);
}

void SlotsMain::inputLoop()
{
	// ezButtons loops
	increaseBet.loop();
	decreaseBet.loop();
	startLever.loop();
}

/**
 * Sets initial pin modes for the Mega 2560.
 */
void SlotsMain::ioSetup()
{
	startLever.setDebounceTime(EZBTNDEBOUNCE);
	increaseBet.setDebounceTime(EZBTNDEBOUNCE);
	decreaseBet.setDebounceTime(EZBTNDEBOUNCE);
}

#pragma endregion
