
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

// ezLED lockLED1{0};
// ezLED lockLED2(lockLEDPins[1]);

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

void SlotsMain::ioSetup()
{
	// Set input pin modes

	// pinMode(leverButtonPin, INPUT_PULLUP);
	// pinMode(increaseBetPin, INPUT_PULLUP);
	// pinMode(decreaseBetPin, INPUT_PULLUP);
	// pinMode(lockButtonPins[0], INPUT_PULLUP);
	// pinMode(lockButtonPins[1], INPUT_PULLUP);
	// pinMode(lockButtonPins[2], INPUT_PULLUP);

	// Set output pin modes

	pinMode(servoPin, OUTPUT);
	pinMode(signalLED1Gnd, OUTPUT);
	pinMode(signalLED2Gnd, OUTPUT);
	pinMode(signalLED1Pin, OUTPUT);
	pinMode(signalLED2Pin, OUTPUT);
	// pinMode(lockLEDPins[0], OUTPUT);
	// pinMode(lockLEDPins[1], OUTPUT);
	// pinMode(lockLEDPins[2], OUTPUT);
	pinMode(motorOutPins[0][0], OUTPUT);
	pinMode(motorOutPins[0][1], OUTPUT);
	pinMode(motorOutPins[1][0], OUTPUT);
	pinMode(motorOutPins[1][1], OUTPUT);
	pinMode(motorOutPins[2][0], OUTPUT);
	pinMode(motorOutPins[2][1], OUTPUT);

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

	// lockLED1 = ezLED(lockLEDPins[0]);
}

void SlotsMain::inputLoop()
{
	// process ezButtons

	increaseBet.loop();
	decreaseBet.loop();
	startLever.loop();

	// Read ezButtons values

	if(startLever.isPressed()) {
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
	Serial.begin(57600);
	Serial.println("-----------------");

	// Initialize pins

	ioSetup();

	// Setup objects

	ledMatrix.start();
	display.start();
	timer.every(80, updateBet);

	// Perform a home spin

	display.welcome();
	game.Setup();
	game.StartSpin(true);
}

void SlotsMain::Loop()
{
	inputLoop();
	timer.tick();
	game.Loop();
}

#pragma endregion

// ------------------------------------------------------------------------- End
