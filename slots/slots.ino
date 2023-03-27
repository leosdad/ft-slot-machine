
#include "MotorDriver.h"
#include "slots.h"

// ----------------------------------------------------------------------- Types

enum {
	START = 0,
	MEASURING,
	COUNTING,
	STOP,
	WAITING,
};

// -------------------------------------------------------- Constants and macros

#define BAUD_RATE 57600

// ------------------------------------------------------------------- Variables

MotorDriver motor1 = MotorDriver(motor1Ports, encoder1);
MotorDriver motor2 = MotorDriver(motor2Ports, encoder2);
MotorDriver motor3 = MotorDriver(motor3Ports, encoder3);

uint16_t counter1;
uint16_t counter2;
uint16_t counter3;

byte state1 = START;
byte state2 = START;
byte state3 = START;

unsigned long lastChange1 = 0;
unsigned long lastChange2 = 0;
unsigned long lastChange3 = 0;

byte currentSignal1;
byte currentSignal2;
byte currentSignal3;

// ----------------------------------------------------------------------- Setup

void setup()
{
	Serial.begin(BAUD_RATE);
	Serial.println("------------------------------------");
	Serial.println("Slot machine");

	pinMode(leverButton, INPUT_PULLUP);
	pinMode(coinIncrementButton, INPUT_PULLUP);

	pinMode(posSensor1, INPUT_PULLUP);
	pinMode(posSensor2, INPUT_PULLUP);
	pinMode(posSensor3, INPUT_PULLUP);
}

// ------------------------------------------------------------------- Main loop

void loop()
{
	loop1();
	loop2();
	loop3();
}

void loop1()
{
	switch(state1) {
		case START:
			motor1.RotateCCW(minSpeed);
			
			while(digitalRead(posSensor1) == LOW) {
				// Wait until reed switch is opened
			}

			state1 = MEASURING;
			break;

		case MEASURING:
			if(digitalRead(posSensor1) == 0) {
				Serial.println("Marker 1 detected.");
				counter1 = finalSteps;
				currentSignal1 = digitalRead(encoder1);
				state1 = COUNTING;
			}
			break;

		case COUNTING:
			if(((micros() - lastChange1) > debouncePeriod) &&
				(digitalRead(encoder1) != currentSignal1)) {
				lastChange1 = micros();
				currentSignal1 = !currentSignal1;
				if(currentSignal1) {	 // RISING flank
					if(--counter1 == 0) {
						state1 = STOP;
					}
				}
			}
			break;

		case STOP:
			motor1.Brake();
			Serial.println("Stopped 1.");
			state1 = WAITING;
			break;

		case WAITING:
			if(state2 == WAITING && state3 == WAITING) {
				while(digitalRead(leverButton)) {
					// Wait for pushbutton release
				}
				if(!digitalRead(leverButton)) {
					state1 = START;
					state2 = START;
					state3 = START;
				}
			}
			break;
	}
}

void loop2()
{
	switch(state2) {
		case START:
			motor2.RotateCCW(minSpeed);
			
			while(digitalRead(posSensor2) == LOW) {
				// Wait until reed switch is opened
			}

			state2 = MEASURING;
			break;

		case MEASURING:
			if(digitalRead(posSensor2) == 0) {
				Serial.println("Marker 2 detected.");
				counter2 = finalSteps;
				currentSignal2 = digitalRead(encoder2);
				state2 = COUNTING;
			}
			break;

		case COUNTING:
			if(((micros() - lastChange2) > debouncePeriod) &&
				(digitalRead(encoder2) != currentSignal2)) {
				lastChange2 = micros();
				currentSignal2 = !currentSignal2;
				if(currentSignal2) {	 // RISING flank
					if(--counter2 == 0) {
						state2 = STOP;
					}
				}
			}
			break;

		case STOP:
			motor2.Brake();
			Serial.println("Stopped 2.");
			state2 = WAITING;
			break;

		case WAITING:
			// while(digitalRead(leverButton)) {
			// 	// Wait for pushbutton release
			// }
			// if(!digitalRead(leverButton)) {
			// 	state2 = START;
			// }
			break;
	}
}

void loop3()
{
	switch(state3) {
		case START:
			motor3.RotateCCW(minSpeed);
			
			while(digitalRead(posSensor3) == LOW) {
				// Wait until reed switch is opened
			}

			state3 = MEASURING;
			break;

		case MEASURING:
			if(digitalRead(posSensor3) == 0) {
				Serial.println("Marker 3 detected.");
				counter3 = finalSteps;
				currentSignal3 = digitalRead(encoder3);
				state3 = COUNTING;
			}
			break;

		case COUNTING:
			if(((micros() - lastChange3) > debouncePeriod) &&
				(digitalRead(encoder3) != currentSignal3)) {
				lastChange3 = micros();
				currentSignal3 = !currentSignal3;
				if(currentSignal3) {	 // RISING flank
					if(--counter3 == 0) {
						state3 = STOP;
					}
				}
			}
			break;

		case STOP:
			motor3.Brake();
			Serial.println("Stopped 3.");
			state3 = WAITING;
			break;

		case WAITING:
			// while(digitalRead(leverButton)) {
			// 	// Wait for pushbutton release
			// }
			// if(!digitalRead(leverButton)) {
			// 	state3 = START;
			// }
			break;
	}
}