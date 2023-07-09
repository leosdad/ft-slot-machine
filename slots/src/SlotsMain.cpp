
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

// ------------------------------------------------------------------- Variables

MotorDriver motor[] = {
	MotorDriver(motor1Out, encoder[0]),
	MotorDriver(motor2Out, encoder[1]),
	MotorDriver(motor3Out, encoder[2])
};

// Inicialize ezButtons
ezButton leverBtn(leverButton);
ezButton increaseBtn(increaseBet);
ezButton decreaseBtn(decreaseBet);
ezButton sensor[NREELS] = {posSensor[0], posSensor[1], posSensor[2]};
ezButton reelBtn[NREELS] = {reelButton[0], reelButton[1], reelButton[2]};

bool spinning;
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
uint16_t payoff = 0;
unsigned long lastChange[NREELS];
char displayBuffer[DISPLAYCHARS];

bool lockReel[NREELS] = {false, false, false};

// Used for debugging
const char *symbolNames[NSYMBOLS] = {"Sevn", "Bana", "Chry", "Meln", "Bell", "Orng", "Lemn", "Grap"};

// ----------------------------------------------------------------------- Setup

void SlotsMain::Setup()
{
	// serialSetup();
	sevenSegSetup();
	ioSetup();
	oledSetup();

	Display::U2s(displayBuffer, payoff);
	Display::Show(displayBuffer);

	startReels(true);
}

// ------------------------------------------------------------------- Main loop

void SlotsMain::Loop()
{
	increaseBtn.loop();
	decreaseBtn.loop();
	leverBtn.loop();
	reelBtn[0].loop();
	reelBtn[1].loop();
	reelBtn[2].loop();

	if(spinning) {

		processReel(0);
		processReel(1);
		processReel(2);

		if(!digitalRead(reelButton[0]) && !digitalRead(reelButton[1])) {

			// Emergency stop
			stopReels();
			displayIdle("Aborted ");

		} else if(isIdle()) {

			displayIdle("Stopped ");
			// calculatePayoff();
			Display::U2s(displayBuffer, payoff);
			Display::Show(displayBuffer);

		}

	} else {

		if(leverBtn.isPressed()) {
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
	sensor[n].loop();

	switch(state[n]) {

		case START:
			rotations[n] = extraTurns[n];
			speed[n] = reelSpeed[n];
			motor[n].RotateCW(speed[n]);
			state[n] = SENSING;
			break;

		case SENSING:
			if(sensor[n].isReleased()) {
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

	calculatePayoff();
	showReelPreview();
	Display::U2s(displayBuffer, payoff);
	Display::Show(displayBuffer);
	oledPrintS(0, 0, "Spinning");

	// Starts the wheels

	spinning = true;
	resetVars(START);
}

/**
 * Looks for a payoff combination from the reel positions.
 */
void SlotsMain::calculatePayoff()
{
	for(int p = 0; p < NPAYOFFS; p++) {
		if( ((payoffs[p][0] == -1) || (reels[0][pos[0]] == (int16_t)payoffs[p][0])) &&
			((payoffs[p][1] == -1) || (reels[1][pos[1]] == (int16_t)payoffs[p][1])) &&
			((payoffs[p][2] == -1) || (reels[2][pos[2]] == (int16_t)payoffs[p][2])) ) {

			// Found a payoff combination

			payoff = payoffs[p][3];
			return;
		}
	}
	payoff = 0;
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
	oledPrintN(1, 1, extraTurns[0]);
	oledPrintN(1, 6, extraTurns[1]);
	oledPrintN(1, 11, extraTurns[2]);

	oledPrintS(2, 1, "   ");
	oledPrintN(2, 1, payoff);

	oledPrintS(3, 1, symbolNames[reels[0][pos[0]]]);
	oledPrintS(3, 6, symbolNames[reels[1][pos[1]]]);
	oledPrintS(3, 11,symbolNames[reels[2][pos[2]]]);
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
	// TODO: remove those that use ezButton, MotorDriver etc.

	leverBtn.setDebounceTime(50);
	increaseBtn.setDebounceTime(50);
	decreaseBtn.setDebounceTime(50);
	reelBtn[0].setDebounceTime(50);
	reelBtn[1].setDebounceTime(50);
	reelBtn[2].setDebounceTime(50);

	pinMode(encoder[0], INPUT_PULLUP);
	pinMode(encoder[1], INPUT_PULLUP);
	pinMode(encoder[2], INPUT_PULLUP);

	pinMode(motor1Out[0], OUTPUT);
	pinMode(motor1Out[1], OUTPUT);
	pinMode(motor2Out[0], OUTPUT);
	pinMode(motor2Out[1], OUTPUT);
	pinMode(motor3Out[0], OUTPUT);
	pinMode(motor3Out[1], OUTPUT);
	pinMode(redLED1[0], OUTPUT);
	pinMode(redLED1[0], OUTPUT);
	pinMode(redLED2[1], OUTPUT);
	pinMode(redLED2[1], OUTPUT);
	pinMode(lockLED[0], OUTPUT);
	pinMode(lockLED[1], OUTPUT);
	pinMode(lockLED[2], OUTPUT);
}

// ------------------------------------------------------------------------- End
