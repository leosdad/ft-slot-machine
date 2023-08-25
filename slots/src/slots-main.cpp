
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include <arduino-timer.h>
#include "drivers/led-matrix.h"
#include "drivers/pwm-micros.h"
#include "slots-main.h"
#include "game.h"
#include "reel.h"
#include "display.h"
#include "locks.h"

// ------------------------------------------------------------ Global variables

Game game;
Locks locks;
LedMatrix ledMatrix;
Display display;
uint8_t lastBet = 255;
auto updateTimer = timer_create_default();
auto lockLedsTimer = timer_create_default();
char *nextMessage = "";
bool frozen = false;

// ------------------------------------------------------------ Global functions

bool updateBet(void *)
{
	if(game.currentBet != lastBet) {
		display.displayBet(game.currentBet);
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
 * Called once when the spin ends.
 */
void endSpin()
{
	frozen = false;
	display.show(nextMessage);
}

// ---------------------------------------------------- Private member functions

void SlotsMain::ioSetup()
{
	// Set output pin modes

	pinMode(servoPin, OUTPUT);
	pinMode(signalLED1Gnd, OUTPUT);
	pinMode(signalLED2Gnd, OUTPUT);
	pinMode(signalLED1Pin, OUTPUT);
	pinMode(signalLED2Pin, OUTPUT);

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
		if(startLever.isPressed()) {
			display.show("Spin ");
			nextMessage = "Done ";
			#if !SPEEDUP
			delay(500);	// TODO: Replace with non-blocking timer
			#endif
			frozen = true;
			// locks.Freeze();
			game.StartSpin(false);
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

	ledMatrix.start();
	display.start();
	display.show("Hello");
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
	locks.Loop(spinning);
	if(spinning != lastSpinning) {
		if(!spinning) {
			endSpin();
		}
		lastSpinning = spinning;
	}
}

#pragma endregion

// ------------------------------------------------------------------------- End
