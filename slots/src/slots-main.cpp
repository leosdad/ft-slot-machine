
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include <arduino-timer.h>
#include "drivers/led-matrix.h"
#include "drivers/pwm-micros.h"
#include "cheering.h"
#include "display.h"
#include "game.h"
#include "locks.h"
#include "reel.h"
#include "slots-main.h"

// ------------------------------------------------------------ Global variables

// Global objects

Game game;
Locks locks;
LedMatrix ledMatrix;
Display display;
Cheering cheers;

auto updateTimer = timer_create_default();

// Global variables

uint8_t lastBet = 255;
uint8_t lastCoins = STARTCOINS;
bool firstSpin = true;
bool reelsLocked = false;

// ------------------------------------------------------------ Global functions

bool updateDisplay()
{
	if(game.currentBet == 0) {
		display.scrollAll("No bet");
	} else {
		display.showBet(game.currentBet);
		if(game.spinPayoff) {
			ledMatrix.printText("\x1d", 9);	// Right pointer
		}
		display.show(game.nCoins, !game.spinPayoff);
	}
}

bool updateBet(void *)
{
	if(game.currentBet != lastBet) {
		updateDisplay();
		lastBet = game.currentBet;
	}
	
	return true;
}

/**
 * Called once each time a spin ends.
 */
void endSpin()
{
	static const char* feats[] = {"", "Sweep", "Bonus ", "Jckpot!"};

	if(firstSpin) {
		display.scroll("Start");
	} else {
		updateDisplay();

		CheerLevel cheerLevel;

		if(game.lastFeature > SpecialFeatures::NONE) {
			cheerLevel = CheerLevel::BIG_WIN;
			display.scrollAll(feats[(uint16_t)game.lastFeature]);
		} else if(game.nCoins > lastCoins) {
			cheerLevel = CheerLevel::WIN;
		} else if(game.nCoins == lastCoins) {
			cheerLevel = CheerLevel::DRAW;
		} else {	// game.nCoins < lastCoins
			cheerLevel = CheerLevel::NONE;
		}

		cheers.Start(cheerLevel);
	}
}

/**
 * Called when a new spin is about to start.
 */
void spin()
{
	cheers.Stop();

	if(game.currentBet == 0) {
		return;
	}

	if(game.nCoins == 0) {
		display.scrollAll("Empty");
		return;
	}

	lastCoins = game.nCoins;
	reelsLocked = false;
	for(int i = 0; i < NREELS; i++) {
		if(locks.IsLocked(i)) {
			reelsLocked = true;
		}
	}

	display.scroll("Spin ");
	#if !SPEEDUP
	delay(500);
	#endif
	game.StartSpin(false);
}

// ---------------------------------------------------- Private member functions

void SlotsMain::ioSetup()
{
	// Set output pin modes

	pinMode(servoPin, OUTPUT);
	pinMode(signalLED1Gnd, OUTPUT);
	pinMode(signalLED2Gnd, OUTPUT);

	// Set fixed levels

	digitalWrite(signalLED1Gnd, LOW);
	digitalWrite(signalLED2Gnd, LOW);

	// Initialize ezButtons

	startLever = ezButton(leverButtonPin);
	increaseBet = ezButton(increaseBetPin);
	decreaseBet = ezButton(decreaseBetPin);

	startLever.setDebounceTime(EZBTNDEBOUNCE);
	increaseBet.setDebounceTime(EZBTNDEBOUNCE);
	decreaseBet.setDebounceTime(EZBTNDEBOUNCE);
}

void SlotsMain::inputLoop()
{
	// process ezButtons

	increaseBet.loop();
	decreaseBet.loop();
	startLever.loop();

	// Read ezButtons values

	if(!game.spinning) {
		if(startLever.isReleased()) {
			firstSpin = false;
			spin();
		} else if(increaseBet.isPressed()) {
			game.ChangeBet(1);
		} else if(decreaseBet.isPressed()) {
			game.ChangeBet(-1);
		}
	}
}

// ----------------------------------------------------- Public member functions

void SlotsMain::Setup()
{
	Serial.begin(BAUDRATE);
	Serial.println("Slots game started.");
	Serial.println();

	// Initialize pins

	ioSetup();

	// Sets up objects

	ledMatrix.setup();
	display.setup();
	display.scroll(" Wait");
	cheers.Setup();
	updateTimer.every(UPDATEBET, updateBet);
	locks.Setup();
	game.Setup();

	// Perform a first (home) spin

	game.StartSpin(true);
}

void SlotsMain::Loop()
{
	updateTimer.tick();
	inputLoop();
	if(game.Loop()) {
		endSpin();
	}
	locks.Loop(
		!game.spinning,
		!(firstSpin || game.spinPayoff || reelsLocked),
		game.currentBet
	);
	cheers.Loop(!game.spinning && game.spinPayoff);
}

#pragma endregion

// ------------------------------------------------------------------------- End
