
// -----------------------------------------------------------------------------

// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "slots.h"

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

MotorDriver motor[] = { MotorDriver(motor1Out, encoder[0]), MotorDriver(motor2Out, encoder[1]), MotorDriver(motor3Out, encoder[2])};

ezButton lever(leverButton);
ezButton toggle0(toggle[0]);
ezButton sensor[NREELS] = {posSensor[0], posSensor[1], posSensor[2]};

bool spinning;
byte currentSignal[NREELS];
byte state[NREELS];
uint8_t extraTurns[] = {0, 0, 0};	// Number of extra 360° revolutions per wheel
uint8_t rotations[NREELS];
uint8_t speed[NREELS];
uint16_t counter[NREELS];
uint16_t finalSteps[] = {0, 0, 0};	// Steps after sensor is triggered
uint16_t pos[] = {0, 0, 0};			// Symbol to be displayed (0-11)
uint16_t money = 0;
unsigned long lastChange[NREELS];
const char *symbolNames[NSYMBOLS] = {"Sevn", "Bana", "Chry", "Wtml", "Bell", "Orng", "Lemn", "Grap"};
char displayBuffer[DISPLAYCHARS];

// ----------------------------------------------------------------------- Setup

void setup()
{
	// Serial.begin(BAUD_RATE);
	// Serial.println("------------------------------------");
	// Serial.println("Slot machine");

	ioSetup();
	oledSetup();

	Display::U2s(displayBuffer, money);
	Display::Show(displayBuffer);

	startReels(true);
}

// ------------------------------------------------------------------- Main loop

void loop()
{
	toggle0.loop();
	lever.loop();

	if(spinning) {

		processReel(0, toggle[0]);
		processReel(1, toggle[0]);
		processReel(2, toggle[0]);

		if(toggle0.isPressed()) {
			stopReels(true, "Aborted ");
		} else if(isIdle()) {
			stopReels(false, "Stopped ");
			calculatePayoff();
			Display::U2s(displayBuffer, money);
			Display::Show(displayBuffer);
		}
	} else {
		if(toggle0.isPressed() || lever.isPressed()) {
			startReels(false);
		}
	}
}

// ------------------------------------------------------------------- Functions

void processReel(int n, int button)
{
	sensor[n].loop();

	switch(state[n]) {
		case START:
			rotations[n] = extraTurns[n];
			speed[n] = normalSpeed[n];
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

void startReels(bool home)
{
	if(home) {

		// Move reels to home position

		extraTurns[0] = extraTurns[1] = extraTurns[2] = 0;
		pos[0] = pos[1] = pos[2] = 0;

	} else {

		// Sets the amount of initial full turns per reel
		
		extraTurns[0] = TrueRandom.random(0, 2);
		extraTurns[1] = TrueRandom.random(extraTurns[0] + 1, 3);
		extraTurns[2] = TrueRandom.random(extraTurns[1] + 1, 4);
		/* To speed up tests */ extraTurns[0] = extraTurns[1] = extraTurns[2] = 0;

		// Draws the final position for each wheel

		pos[0] = TrueRandom.random(NREELSYMBOLS);
		pos[1] = TrueRandom.random(NREELSYMBOLS);
		pos[2] = TrueRandom.random(NREELSYMBOLS);
		// /* For speed calibration */ pos[0] = pos[1] = pos[2] = 0;
	}

	// Calculates the number of steps necessary to reach each position

	finalSteps[0] = stepOffsets[pos[0]];
	finalSteps[1] = stepOffsets[pos[1]];
	finalSteps[2] = stepOffsets[pos[2]];

	// DEBUG

	showReelPreview();
	calculatePayoff();
	Display::U2s(displayBuffer, money);
	Display::Show(displayBuffer);

	digitalWrite(lockLED[0], HIGH);
	spinning = true;
	state[0] = state[1] = state[2] = START;
	resetVars();

	oledPrintS(0, 0, "Spinning");
}

void showReelPreview()
{
	oledPrintS(1, 2, " ");
	oledPrintN(1, 1, pos[0]);
	oledPrintS(1, 7, " ");
	oledPrintN(1, 6, pos[1]);
	oledPrintS(1, 12, " ");
	oledPrintN(1, 11, pos[2]);

	oledPrintN(2, 1, reels[0][pos[0]]);
	oledPrintN(2, 6, reels[1][pos[1]]);
	oledPrintN(2, 11, reels[2][pos[2]]);

	oledPrintS(3, 1, symbolNames[reels[0][pos[0]]]);
	oledPrintS(3, 6, symbolNames[reels[1][pos[1]]]);
	oledPrintS(3, 11,symbolNames[reels[2][pos[2]]]);
}

/**
 * Looks for a payoff combination from the reel positions.
 */
void calculatePayoff()
{
	for(int p = 0; p < NPAYOFFS; p++) {
		if( ((payoffs[p][0] == -1) || (reels[0][pos[0]] == (int16_t)payoffs[p][0])) &&
			((payoffs[p][1] == -1) || (reels[1][pos[1]] == (int16_t)payoffs[p][1])) &&
			((payoffs[p][2] == -1) || (reels[2][pos[2]] == (int16_t)payoffs[p][2])) ) {

			// Found a payoff combination

			money = payoffs[p][3];
			return;
		}
	}
	money = 0;
}

void stopReels(bool coast, char *msg)
{
	if(coast) {
		motor[0].Coast();
		motor[1].Coast();
		motor[2].Coast();
	}
	digitalWrite(lockLED[0], LOW);
	spinning = false;
	state[0] = state[1] = state[2] = START;
	resetVars();

	if(msg) {
		oledPrintS(0, 0, msg);
	}
}

bool isIdle()
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

void resetVars()
{
	for(int i = 0; i < NREELS; i++) {
		MotorDriver(motor1Out, encoder[0]), counter[i] = 0;
		currentSignal[i] = 0;
		lastChange[i] = 0;
		state[i] = START;
		speed[i] = normalSpeed[i];
		rotations[i] = 0;
	}
}

void ioSetup()
{
	// Reset seven-segment display

	Wire.begin();
	Display::Init();
	Display::Clear();
	Display::Stop();

	// Set pin modes

	pinMode(decreaseBet, INPUT_PULLUP);
	pinMode(increaseBet, INPUT_PULLUP);
	pinMode(leverButton, INPUT_PULLUP);
	pinMode(toggle[0], INPUT_PULLUP);
	pinMode(toggle[1], INPUT_PULLUP);
	pinMode(toggle[2], INPUT_PULLUP);
	pinMode(encoder[0], INPUT_PULLUP);
	pinMode(encoder[1], INPUT_PULLUP);
	pinMode(encoder[2], INPUT_PULLUP);

	pinMode(posSensor[0], INPUT);
	pinMode(posSensor[1], INPUT);
	pinMode(posSensor[2], INPUT);

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
