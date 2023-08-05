
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#pragma region -------------------------------------------------------- Includes

#include <ezButton.h>
#include <TrueRandom.h>

#include "slots-main.h"
#include "seven-seg.h"
#include "motor-driver.h"
#include "game.h"
#include "reel.h"

#pragma endregion

#pragma region ------------------------------------------------ Global variables

// EzButtons

ezButton startLever(leverButtonPin);
ezButton increaseBet(increaseBetPin);
ezButton decreaseBet(decreaseBetPin);

// Slot machine variabless

Game game;

// Other variabless

SevenSeg sevenSegDisplay;
OledDisplay od;

#pragma endregion

#pragma region ---------------------------------------- Private member functions

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
	od.DisplayPayoff(0);
	game.StartSpin(home);
	if(game.playing) {
		od.DisplayBet(game.currentBet);
		if(!home) {
			sevenSegDisplay.DisplayNumber(game.nCoins);
		}
	}
	od.DisplayDebugInfo(game);
	od.ShowState("Spinning");
}

void SlotsMain::stopSpin()
{
	game.StopSpin();
	od.DisplayBet(game.SetBet(game.currentBet));
	od.ShowState("Stopped ");
	sevenSegDisplay.DisplayNumber(game.nCoins);
	od.DisplayPayoff(game.spinPayoff);
}

void SlotsMain::inputLoop()
{
	// ezButtons

	increaseBet.loop();
	decreaseBet.loop();
	startLever.loop();
}

void SlotsMain::ioSetup()
{
	// Sets initial pin modes for the Mega 2560

	digitalWrite(signalLED1Gnd, LOW);
	digitalWrite(signalLED2Gnd, LOW);

	// Initialize ezButtons

	startLever.setDebounceTime(EZBTNDEBOUNCE);
	increaseBet.setDebounceTime(EZBTNDEBOUNCE);
	decreaseBet.setDebounceTime(EZBTNDEBOUNCE);
}

#pragma endregion

#pragma region ----------------------------------------- Public member functions

void SlotsMain::Setup()
{
	// Initialize objects

	sevenSegDisplay.Setup();
	sevenSegDisplay.ScrollMessage("HELLO   ");
	ioSetup();
	game.Setup();
	game.InitReels(motorOutPins, encoderPins, homeSensorPins,
		lockButtonPins, lockLEDPins, motorSpeed);
	od.Setup(DEBUGINFO);

	// Reset reels to home position and start game with initial values

	game.nCoins = STARTCOINS;
	od.DisplayPayoff(game.spinPayoff);
	game.SetBet(0);
	od.DisplayBet(game.currentBet);
	game.SetBet(INITIALBET);
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

#pragma endregion
