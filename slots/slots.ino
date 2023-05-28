
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

MotorDriver motor[] = {
	MotorDriver(motor1Out, encoder[0]),
	MotorDriver(motor2Out, encoder[1]),
	MotorDriver(motor3Out, encoder[2])};

bool spinning;
uint16_t counter[NREELS];
byte currentSignal[NREELS];
unsigned long lastChange[NREELS];
byte state[NREELS];
uint8_t speed[NREELS];
uint8_t rotations[NREELS];

ezButton lever(toggle[0]);
ezButton sensor[NREELS] = {
	posSensor[0],
	posSensor[1],
	posSensor[2]};

// ----------------------------------------------------------------------- Setup

void setup()
{
	// Serial.begin(BAUD_RATE);
	// Serial.println("------------------------------------");
	// Serial.println("Slot machine");

	ioSetup();
	oledSetup();
	oledPrint(0, 0, "Slot machine");
	startReels();
	oledPrint(1, 0, "Spinning");
}

// ------------------------------------------------------------------- Main loop

void loop()
{
	lever.loop();

	if(spinning) {
		processReel(0, toggle[0]);
		processReel(1, toggle[0]);
		processReel(2, toggle[0]);
		if(lever.isPressed()) {
			motor[0].Coast();
			motor[1].Coast();
			motor[2].Coast();
			stopReels();
			oledPrint(1, 0, "Aborted ");
		} else if(isIdle()) {
			stopReels();
			oledPrint(1, 0, "Stopped ");
		}
	} else {
		if(lever.isPressed()) {
			startReels();
			oledPrint(1, 0, "Spinning");
		}
	}
}

// ------------------------------------------------------------------- Functions

void processReel(int n, int button)
{
	sensor[n].loop();
	
	switch(state[n]) {
		case START:
			rotations[n] = extraRotations[n];
			speed[n] = normalSpeed[n];
			motor[n].RotateCW(speed[n]);
			state[n] = SENSING;
			break;

		case SENSING:
			if(sensor[n].isPressed()) {
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

void startReels()
{
	digitalWrite(lockLED[0], true);
	spinning = true;
	state[0] = state[1] = state[2] = START;
	reset();
}

void stopReels()
{
	digitalWrite(lockLED[0], false);
	spinning = false;
	state[0] = state[1] = state[2] = START;
	reset();
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

void reset()
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
	pinMode(decreaseBet, INPUT_PULLUP);
	pinMode(increaseBet, INPUT_PULLUP);
	pinMode(leverButton, INPUT_PULLUP);

	pinMode(toggle[0], INPUT_PULLUP);
	pinMode(toggle[1], INPUT_PULLUP);
	pinMode(toggle[2], INPUT_PULLUP);

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
