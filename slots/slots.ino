
#include "MotorDriver.h"
#include "slots.h"
#include <Wire.h>
// #include <LiquidCrystal_I2C.h>

// ----------------------------------------------------------------------- Types

enum {
	START = 0,
	SENSING,
	COUNTING,
	STOP,
	WAITING,
};

// -------------------------------------------------------- Constants and macros

#define BAUD_RATE 57600

// ------------------------------------------------------------------- Variables

MotorDriver motor[] = {
	MotorDriver(motor1Ports, encoder[0]),
	MotorDriver(motor2Ports, encoder[1]),
	MotorDriver(motor3Ports, encoder[2])
};

uint16_t counter[NREELS];
byte currentSignal[NREELS];
unsigned long lastChange[] = {0, 0, 0};
byte state[] = { START, START, START };
uint8_t speed[] = {normalSpeed[0], normalSpeed[1], normalSpeed[2]};
uint8_t rotations[NREELS];// = {0, 0, 0};

// LiquidCrystal_I2C lcd(0x27, 16, 2);

// ----------------------------------------------------------------------- Setup

void setup()
{
	Serial.begin(BAUD_RATE);
	Serial.println("------------------------------------");
	Serial.println("Slot machine");

	pinMode(leverButton, INPUT_PULLUP);
	pinMode(coinIncrementButton, INPUT_PULLUP);

	// lcd.init();
	// lcd.backlight();      // Make sure backlight is on
	// lcd.clear();
	// lcd.setCursor(0, 0);
	// lcd.print("Slot machine");

	for(int n = 0; n < NREELS; n++) {
		rotations[n] = extraRotations[n];
	}
}

// ------------------------------------------------------------------- Main loop

void __loop()
{
	motor[0].RotateCCW(80);
	Serial.println("Motor 1 started");
	delay(1000);

	motor[1].RotateCCW(80);
	Serial.println("Motor 2 started");
	delay(1000);

	motor[2].RotateCCW(80);
	Serial.println("Motor 3 started");
	delay(1000);

	motor[0].Brake();
	Serial.println("Motor 1 stopped");
	delay(1000);

	motor[1].Brake();
	Serial.println("Motor 2 stopped");
	delay(1000);

	motor[2].Brake();
	Serial.println("Motor 3 stopped");
	delay(1000);
}

void loop()
{
	processReel(0);
	processReel(1);
	processReel(2);
}

void processReel(int n)
{
	switch(state[n]) {
		case START:
			rotations[n] = extraRotations[n];
			speed[n] = normalSpeed[n];
			motor[n].RotateCCW(speed[n]);
			while(digitalRead(posSensor[n])) {
				// Wait while sensor is blocked
			}
			state[n] = SENSING;
			break;

		case SENSING:
			if(digitalRead(posSensor[n])) {
				counter[n] = finalSteps[n];
				currentSignal[n] = digitalRead(encoder[n]);
				if(rotations[n] > 0) {
					rotations[n]--;
					// while(digitalRead(posSensor[n])) {
					// 	// Wait while sensor is blocked
					// }
				} else {
					// speed[n] = max(speed[n] - speedDecrement, minSpeed[n]);
					// speed[n] = minSpeed[n];
					// motor[n].RotateCCW(speed[n]);
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
					if(counter[n]-- == 0) {
						state[n] = STOP;
					}
				}
			}
			break;

		case STOP:
			motor[n].Brake();
			state[n] = WAITING;
			break;

		case WAITING:
			if(state[0] == WAITING && state[1] == WAITING && state[2] == WAITING) {
				while(digitalRead(leverButton) == HIGH) {
					// Wait for pushbutton release
				}
				if(digitalRead(leverButton) == LOW) {
					state[0] = state[1] = state[2] = START;
					// lcd.setCursor(0, 0);
					// lcd.print("----  ----  ----");
					// lcd.setCursor(0, 1);
					// lcd.print("                ");
				}
			}
			break;
	}
}
