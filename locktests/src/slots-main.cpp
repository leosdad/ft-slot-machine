
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include <arduino-timer.h>
#include "locks.h"
#include "slots-main.h"
#include "slots.h"

// ------------------------------------------------------------ Global variables

// Global objects

Locks locks;

auto updateTimer = timer_create_default();
auto lockBlink = timer_create_default();
auto lockTurnOff = timer_create_default();

// Global variables

uint8_t lastBet = 255;
bool frozen = false;
bool firstSpin = true;
bool lockBlinkState = false;
uint8_t currentBet;

// ------------------------------------------------------------ Global functions

bool updateDisplay()
{
	if(currentBet == 0) {
		Serial.println("No bet");
	} else {
		Serial.println("CurrentBet: " + String(currentBet));
	}
}

bool updateBet(void *)
{
	if(currentBet != lastBet) {
		updateDisplay();
		lastBet = currentBet;
	}
	
	return true;
}

bool LockLEDsOff(void *)
{
	lockBlinkState = false;
	return true;
}

bool blinkLockLEDs(void *)
{
	if(!lockBlinkState) {
		lockBlinkState = true;
		lockTurnOff.in(LOCKBLINKON, LockLEDsOff);
	}
	return true;
}

/**
 * Called once each time a spin ends.
 */
void endSpin()
{
	Serial.println("End spin");

	frozen = false;

	if(firstSpin) {
		firstSpin = false;
	}
}

/**
 * Called when a new spin is about to start.
 */
void spin()
{
	if(currentBet == 0) {
		return;
	}

	frozen = true;
	Serial.println("Spining");
}

// ---------------------------------------------------- Private member functions

void SlotsMain::ioSetup()
{
	// Initialize ezButtons

	increaseBet = ezButton(increaseBetPin);
	decreaseBet = ezButton(decreaseBetPin);
	increaseBet.setDebounceTime(EZBTNDEBOUNCE);
	decreaseBet.setDebounceTime(EZBTNDEBOUNCE);
}

void SlotsMain::inputLoop()
{
	// process ezButtons

	increaseBet.loop();
	decreaseBet.loop();

	// Read ezButtons values

	if(increaseBet.isPressed()) {
		currentBet = constrain(currentBet + 1, 0, MAXBET);
	} else if(decreaseBet.isPressed()) {
		currentBet = constrain(currentBet - 1, 0, MAXBET);
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
	locks.Setup();

	// Sets up objects

	updateTimer.every(UPDATEBET, updateBet);
	lockBlink.every(LOCKBLINK, blinkLockLEDs);

	// Perform a first (home) spin

	currentBet = INITIALBET;
}

void SlotsMain::Loop()
{
	updateTimer.tick();
	lockBlink.tick();
	lockTurnOff.tick();

	inputLoop();
	locks.Loop();
}

#pragma endregion

// ------------------------------------------------------------------------- End
