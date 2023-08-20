
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include <ezButton.h>
#include <arduino-timer.h>

#include "slots-main.h"
#include "led-matrix.h"
#include "game.h"
#include "reel.h"
#include "display.h"

// ------------------------------------------------------------ Global variables

Game game;
LedMatrix ledMatrix;
Display display;
auto timer = timer_create_default();
ezButton startLever(leverButtonPin);
ezButton increaseBet(increaseBetPin);
ezButton decreaseBet(decreaseBetPin);

uint8_t lastBet = 255;

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
	pinMode(lockButtonPins[0], INPUT_PULLUP);
	pinMode(lockButtonPins[1], INPUT_PULLUP);
	pinMode(lockButtonPins[2], INPUT_PULLUP);

	// Set output pin modes

	pinMode(servoPin, OUTPUT);
	pinMode(signalLED1Gnd, OUTPUT);
	pinMode(signalLED2Gnd, OUTPUT);
	pinMode(signalLED1Pin, OUTPUT);
	pinMode(signalLED2Pin, OUTPUT);
	pinMode(lockLEDPins[0], OUTPUT);
	pinMode(lockLEDPins[1], OUTPUT);
	pinMode(lockLEDPins[2], OUTPUT);
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

	if(startLever.isPressed()) {
		Serial.println("Start");
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
	// gameMachine.Setup();
	// mgr.addListener(new EvtTimeListener(80, true, (EvtAction)updateBet));
	timer.every(80, updateBet);
	// Start game proper

	game.Setup();
	display.welcome();
	game.InitReels(motorOutPins, encoderPins, homeSensorPins,
	 	lockButtonPins, lockLEDPins, motorSpeed);

	// Reset reels to home position and start game with initial values

	game.nCoins = STARTCOINS;
	// display.DisplayPayoff(game.spinPayoff);
	// game.SetBet(0);
	game.SetBet(INITIALBET);
	game.StartSpin(true);
}

void SlotsMain::Loop()
{
	inputLoop();
	timer.tick();
}

#pragma endregion

// ------------------------------------------------------------------------- End
