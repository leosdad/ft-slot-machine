
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
auto lockLedsTimer = timer_create_default();

// Global variables

uint8_t lastBet = 255;
bool frozen = false;
bool firstSpin = true;

// ------------------------------------------------------------ Global functions

bool updateBet(void *)
{
	if(game.currentBet != lastBet) {
		locks.CalcLocked();
		if(game.currentBet == 0) {
			display.showFull("No bet");
		} else {
			display.showBet(game.currentBet);
			display.show(game.nCoins);
		}
		lastBet = game.currentBet;
	}
	
	return true;
}

bool blinkLockLEDs(void *)
{
	locks.ledState = frozen ? LOW : !locks.ledState;
	return true;
}

/**
 * Called once each time a spin ends.
 */
void endSpin()
{
	frozen = false;

	if(firstSpin) {
		locks.AllowOrBlock(false);
		locks.CalcLocked();
		display.show("Start");
		firstSpin = false;
	} else {
		locks.AllowOrBlock(game.spinPayoff == 0);
		locks.CalcLocked();
		display.show(game.nCoins);
		cheers.Start();
	}
}

/**
 * Called when a new spin is about to start.
 */
void startSpin()
{
	cheers.Stop();

	if(game.currentBet == 0) {
		return;
	}

	if(game.nCoins == 0) {
		display.showFull("Empty");
		return;
	}

	frozen = true;
	display.show("Spin ");
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

	if(!spinning) {
		if(startLever.isReleased()) {
			startSpin();
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

	// Setup objects

	ledMatrix.setup();
	display.setup();
	display.show(" Wait");
	cheers.Setup();
	updateTimer.every(UPDATEBET, updateBet);
	lockLedsTimer.every(LOCKBLINK, blinkLockLEDs);

	// Perform a first (home) spin

	locks.Setup();
	game.Setup();
	game.StartSpin(true);
}

void SlotsMain::Loop()
{
	spinning = game.Loop();
	inputLoop();
	updateTimer.tick();
	lockLedsTimer.tick();
	locks.Loop();
	cheers.Loop(!spinning && game.spinPayoff, false);
	if(spinning != lastSpinning) {
		if(!spinning) {
			endSpin();
		}
		lastSpinning = spinning;
	}
}

#pragma endregion

// ------------------------------------------------------------------------- End
