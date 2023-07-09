
// -----------------------------------------------------------------------------

// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "SlotsMain.h"

// Common libraries

#include <Wire.h>
#include <SevenSegDisplay.h>
#include <ezButton.h>
#include <ezOutput.h>
#include <TrueRandom.h>

// Project libraries

#include "MotorDriver.h"
#include "oled-display.h"

// ----------------------------------------------------------------------- Types

enum {
	IDLE = 0,
	START,
	SENSING,
	COUNTING,
};

// --------------------------------------------------------------------- Defines

#define BAUD_RATE 57600

// Macros

#define CALCN1(n)	(n > 0 ? n - 1 : n + NREELSYMBOLS - 1)
#define CALCN2(n)	(n)
#define CALCN3(n)	(n < NREELSYMBOLS - 1 ? n + 1 : 0 )

// Paylines

#define LINE1(n)	(reels[n][CALCN1(pos[n])])
#define LINE2(n)	(reels[n][CALCN2(pos[n])])
#define LINE3(n)	(reels[n][CALCN3(pos[n])])

// ------------------------------------------------------------------- Variables

MotorDriver motor[] = {
	MotorDriver(motor1Out, encoder[0]),
	MotorDriver(motor2Out, encoder[1]),
	MotorDriver(motor3Out, encoder[2])
};

// Inicialize ezButtons
ezButton startLever(leverButtonPin);
ezButton increaseBet(increaseBetPin);
ezButton decreaseBet(decreaseBetPin);
ezButton posSensor[NREELS] = {positionSensor[0], positionSensor[1], positionSensor[2]};
ezButton reelBtn[NREELS] = {reelButtonPin[0], reelButtonPin[1], reelButtonPin[2]};

bool spinning;
bool playing = false;
byte currentSignal[NREELS];
byte state[NREELS];

// Number of extra 360° revolutions per wheel
uint8_t extraTurns[] = {0, 0, 0};
uint8_t rotations[NREELS];
uint8_t speed[NREELS];
uint16_t counter[NREELS];

// Steps after sensor is triggered
uint16_t finalSteps[] = {0, 0, 0};

// Position of symbol to be displayed (0-11)
uint16_t pos[] = {0, 0, 0};
uint16_t totalPayoff = 0;
unsigned long lastChange[NREELS];
char displayBuffer[DISPLAYCHARS];

bool lockReel[NREELS] = {false, false, false};

// Used for debugging
const char *symbolNames[NSYMBOLS] = {"Sevn", "Bana", "Chry", "Mlon", "Bell", "Orng", "Lmon", "Grap"};

// ----------------------------------------------------------------------- Setup

void SlotsMain::Setup()
{
	// serialSetup();
	sevenSegSetup();
	ioSetup();
	oledSetup();

	Display::U2s(displayBuffer, totalPayoff);
	Display::Show(displayBuffer);

	startReels(true);
}

// ------------------------------------------------------------------- Main loop

void SlotsMain::Loop()
{
	increaseBet.loop();
	decreaseBet.loop();
	startLever.loop();
	reelBtn[0].loop();
	reelBtn[1].loop();
	reelBtn[2].loop();

	if(spinning) {

		processReel(0);
		processReel(1);
		processReel(2);

		if(!digitalRead(reelButtonPin[0]) && !digitalRead(reelButtonPin[1])) {

			// Emergency stop
			stopReels();
			displayIdle("Aborted ");

		} else if(isIdle()) {

			displayIdle("Stopped ");
			Display::U2s(displayBuffer, totalPayoff);
			Display::Show(displayBuffer);

		}

	} else {

		// Process buttons

		if(startLever.isPressed()) {
			startReels(false);
		} else if(reelBtn[0].isPressed()) {
			lockReel[0] = !lockReel[0];
			digitalWrite(lockLED[0], lockReel[0]);
		} else if(reelBtn[1].isPressed()) {
			lockReel[1] = !lockReel[1];
			digitalWrite(lockLED[1], lockReel[1]);
		} else if(reelBtn[2].isPressed()) {
			lockReel[2] = !lockReel[2];
			digitalWrite(lockLED[2], lockReel[2]);
		}
	}
}

// --------------------------------------------------------------------- Methods

/**
 * Individual reel state machine.
 */
void SlotsMain::processReel(int n)
{
	posSensor[n].loop();

	switch(state[n]) {

		case START:
			rotations[n] = extraTurns[n];
			speed[n] = reelSpeed[n];
			motor[n].RotateCW(speed[n]);
			state[n] = SENSING;
			break;

		case SENSING:
			if(posSensor[n].isReleased()) {
				if(rotations[n] > 0) {
					rotations[n]--;
				} else {
					counter[n] = finalSteps[n];
					currentSignal[n] = digitalRead(encoder[n]);
					state[n] = COUNTING;
				}
			}
			break;

		case COUNTING:
			if(((micros() - lastChange[n]) > debouncePeriod) &&
				(digitalRead(encoder[n]) != currentSignal[n])) {
				lastChange[n] = micros();
				currentSignal[n] = !currentSignal[n];
				if(currentSignal[n]) {	// RISING flank
					if(counter[n]-- + homeOffset == 0) {
						motor[n].Brake();
						state[n] = IDLE;
					}
				}
			}
			break;
	}
}

/**
 * Does the necessary calculations, draws 3 symbols and starts the reels.
 */
void SlotsMain::startReels(bool home)
{
	if(home) {

		// Move reels to home position

		extraTurns[0] = extraTurns[1] = extraTurns[2] = 0;
		pos[0] = pos[1] = pos[2] = 0;

	} else {

		// Sets the amount of initial full turns per reel
#if SPEEDUP || CALIBRATE
		extraTurns[0] = extraTurns[1] = extraTurns[2] = 0;
#else
		extraTurns[0] = TrueRandom.random(0, 3);
		extraTurns[1] = TrueRandom.random(extraTurns[0] + 1, 4);
		extraTurns[2] = TrueRandom.random(extraTurns[1] + 1, 5);
#endif

		// Draws the final position for each wheel
#if CALIBRATE
		pos[0] = pos[1] = pos[2] = 0;
#else
		pos[0] = TrueRandom.random(NREELSYMBOLS);
		pos[1] = TrueRandom.random(NREELSYMBOLS);
		pos[2] = TrueRandom.random(NREELSYMBOLS);
#endif

	}

	// Calculates the number of steps necessary to reach each position

	finalSteps[0] = stepOffsets[pos[0]];
	finalSteps[1] = stepOffsets[pos[1]];
	finalSteps[2] = stepOffsets[pos[2]];

	// Calculates the payoff for all paylines

	if(playing) {
		totalPayoff = 0;
		totalPayoff += calcPayoff(0);
		totalPayoff += calcPayoff(1);
		totalPayoff += calcPayoff(2);
		showReelPreview();
		Display::U2s(displayBuffer, totalPayoff);
		Display::Show(displayBuffer);
	}

	// Starts the wheels

	playing = true;
	oledPrintS(0, 0, "Spinning");
	spinning = true;
	resetVars(START);
}

/*
 * Returns the current symbol number of the line and reel specified.
 */
uint8_t SlotsMain::getLineSymbol(uint8_t line, uint8_t reel)
{
	switch(line) {
		case 0:
			return LINE1(reel);
		case 1:
			return LINE2(reel);
		case 2:
			return LINE3(reel);
	}
}

/**
 * Looks for a payoff combination from the reel positions.
 */
uint16_t SlotsMain::calcPayoff(int line)
{
	for(int p = 0; p < NPAYOFFS; p++) {
		if( ((payoffs[p][0] == -1) || (getLineSymbol(line, 0) == (int16_t)payoffs[p][0])) &&
			((payoffs[p][1] == -1) || (getLineSymbol(line, 1) == (int16_t)payoffs[p][1])) &&
			((payoffs[p][2] == -1) || (getLineSymbol(line, 2) == (int16_t)payoffs[p][2])) ) {

			// Found a payoff combination

			return payoffs[p][3];
		}
	}
	return 0;
}

/**
 * Force stop all three reels.
 */
void SlotsMain::stopReels()
{
	for(int i = 0; i < NREELS; i++) {
		motor[i].Coast();
	}
}

/**
 * Turns off LEDs and display a message.
 */
void SlotsMain::displayIdle(char *msg)
{
	spinning = false;
	state[0] = state[1] = state[2] = START;
	resetVars(IDLE);

	if(msg) {
		oledPrintS(0, 0, msg);
	}
}

/**
 * Returns true if all reels are in idle state.
 */
bool SlotsMain::isIdle()
{
	if(state[0] != IDLE) {
		return false;
	}
	if(state[1] != IDLE) {
		return false;
	}
	if(state[2] != IDLE) {
		return false;
	}
	return true;
}

/**
 * Resets the three reels prior to the next spin.
 */
void SlotsMain::resetVars(int _state)
{
	for(int i = 0; i < NREELS; i++) {
		MotorDriver(motor1Out, encoder[0]), counter[i] = 0;
		currentSignal[i] = 0;
		lastChange[i] = 0;
		state[i] = _state;
		speed[i] = reelSpeed[i];
		rotations[i] = 0;
	}
}

/**
 * Shows the state and future symbols of the three reels.
 */
void SlotsMain::showReelPreview()
{
	// oledPrintN(1, 1, extraTurns[0]);
	// oledPrintN(1, 6, extraTurns[1]);	
	// oledPrintN(1, 11, extraTurns[2]);

	// oledPrintS(2, 1, "   ");
	// oledPrintN(2, 1, payoff);

	oledPrintN(1, 1,  getLineSymbol(0, 0));
	oledPrintN(1, 3,  getLineSymbol(0, 1));
	oledPrintN(1, 5,  getLineSymbol(0, 2));
	oledPrintN(2, 1,  getLineSymbol(1, 0));
	oledPrintN(2, 3,  getLineSymbol(1, 1));
	oledPrintN(2, 5,  getLineSymbol(1, 2));
	oledPrintN(3, 1,  getLineSymbol(2, 0));
	oledPrintN(3, 3,  getLineSymbol(2, 1));
	oledPrintN(3, 5,  getLineSymbol(2, 2));

	oledPrintS(1, 8, "    ");
	oledPrintN(1, 8, calcPayoff(0));
	oledPrintS(2, 8, "    ");
	oledPrintN(2, 8, calcPayoff(1));
	oledPrintS(3, 8, "    ");
	oledPrintN(3, 8, calcPayoff(2));

	// oledPrintS(1, 1,  symbolNames[LINE1(0) - 1]);
	// oledPrintS(1, 6,  symbolNames[LINE1(1) - 1]);
	// oledPrintS(1, 11, symbolNames[LINE1(2) - 1]);
 	// oledPrintS(2, 1,  symbolNames[LINE2(0) - 1]);
	// oledPrintS(2, 6,  symbolNames[LINE2(1) - 1]);
	// oledPrintS(2, 11, symbolNames[LINE2(2) - 1]);
 	// oledPrintS(3, 1,  symbolNames[LINE3(0) - 1]);
	// oledPrintS(3, 6,  symbolNames[LINE3(1) - 1]);
	// oledPrintS(3, 11, symbolNames[LINE3(2) - 1]);
}

/**
 * Initializes the main serial port.
 */
void SlotsMain::serialSetup()
{
	Serial.begin(BAUD_RATE);
	Serial.println("------------------------------------");
	Serial.println("Slot machine");
}

/**
 * Initializes the seven-segment display.
 */
void SlotsMain::sevenSegSetup()
{
	// Reset seven-segment display

	Wire.begin();
	Display::Init();
	Display::Clear();
	Display::Stop();
}

/**
 * Sets initial pin modes for the Arduino.
 */
void SlotsMain::ioSetup()
{
	startLever.setDebounceTime(50);
	increaseBet.setDebounceTime(50);
	decreaseBet.setDebounceTime(50);
	reelBtn[0].setDebounceTime(50);
	reelBtn[1].setDebounceTime(50);
	reelBtn[2].setDebounceTime(50);

	pinMode(redLED1[0], OUTPUT);
	pinMode(redLED1[0], OUTPUT);
	pinMode(redLED2[1], OUTPUT);
	pinMode(redLED2[1], OUTPUT);
	pinMode(lockLED[0], OUTPUT);
	pinMode(lockLED[1], OUTPUT);
	pinMode(lockLED[2], OUTPUT);
}

// ------------------------------------------------------------------------- End
