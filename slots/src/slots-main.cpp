
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include <arduino-timer.h>

#include "slots-main.h"
#include "drivers/led-matrix.h"
#include "game.h"
#include "reel.h"
#include "display.h"

// ------------------------------------------------------------ Global variables

Game game;
LedMatrix ledMatrix;
Display display;
auto timer = timer_create_default();

uint8_t lastBet = 255;
bool spinning = false;

// ------------------------------------------------------------ Global functions

bool updateBet(void *)
{
	if(game.currentBet != lastBet) {
		display.displayBet(game.currentBet);
		lastBet = game.currentBet;
	}
	return true;
}

// ---------------------------------------------------- Private member functions

// TODO: move most of these to game class

void SlotsMain::ioSetup()
{
	// Set output pin modes

	pinMode(servoPin, OUTPUT);
	pinMode(signalLED1Gnd, OUTPUT);
	pinMode(signalLED2Gnd, OUTPUT);
	pinMode(signalLED1Pin, OUTPUT);
	pinMode(signalLED2Pin, OUTPUT);
	// pinMode(motorOutPins[0][0], OUTPUT);
	// pinMode(motorOutPins[0][1], OUTPUT);
	// pinMode(motorOutPins[1][0], OUTPUT);
	// pinMode(motorOutPins[1][1], OUTPUT);
	// pinMode(motorOutPins[2][0], OUTPUT);
	// pinMode(motorOutPins[2][1], OUTPUT);

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

	if(startLever.isPressed() && !spinning) {
		game.StartSpin(false);
	} else if(increaseBet.isPressed()) {
		game.ChangeBet(1);
	} else if(decreaseBet.isPressed()) {
		game.ChangeBet(-1);
	}
}

// ----------------------------------------------------- Public member functions

void SlotsMain::Setup()
{
	// Serial.begin(BAUDRATE);
	// Serial.println("---- Slots started ----");

	// Initialize pins

	ioSetup();

	// Setup objects

	ledMatrix.start();
	display.start();
	timer.every(80, updateBet);

	// Perform a home spin

	display.welcome();
	game.Setup();
	// updateBet(NULL);
	game.StartSpin(true);
}

void SlotsMain::Loop()
{
	spinning = game.Loop();
	inputLoop();
	timer.tick();
}

#pragma endregion

// ------------------------------------------------------------------------- End
