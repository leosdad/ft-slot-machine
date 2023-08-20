
#include <Arduino.h>

#include "../slots.h"
#include "output-tests.h"

#define BAUDRATE 57600
#define INTERVAL 500

void ioSetup()
{
	// Set input pin modes

	pinMode(leverButtonPin, INPUT_PULLUP);
	pinMode(increaseBetPin, INPUT_PULLUP);
	pinMode(decreaseBetPin, INPUT_PULLUP);
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

	digitalWrite(signalLED1Gnd, LOW);
	digitalWrite(signalLED2Gnd, LOW);
}

void lockLEDTests()
{
	Serial.println("LED 1 on");
	digitalWrite(lockLEDPins[0], HIGH);
	delay(INTERVAL);
	digitalWrite(lockLEDPins[0], LOW);
	delay(INTERVAL);

	Serial.println("LED 2 on");
	digitalWrite(lockLEDPins[1], HIGH);
	delay(INTERVAL);
	digitalWrite(lockLEDPins[1], LOW);
	delay(INTERVAL);

	Serial.println("LED 3 on");
	digitalWrite(lockLEDPins[2], HIGH);
	delay(INTERVAL);
	digitalWrite(lockLEDPins[2], LOW);
	delay(INTERVAL);
}

void signalLEDTests()
{
	Serial.println("Left LED");
	digitalWrite(signalLED1Pin, HIGH);
	delay(INTERVAL);
	digitalWrite(signalLED1Pin, LOW);
	delay(INTERVAL);

	Serial.println("Right LED");
	digitalWrite(signalLED2Pin, HIGH);
	delay(INTERVAL);
	digitalWrite(signalLED2Pin, LOW);
	delay(INTERVAL);
}

void lockLEDTestsAnalog()
{
	Serial.println("LED 1 on (weak)");
	analogWrite(lockLEDPins[0], 15);
	delay(INTERVAL);
	analogWrite(lockLEDPins[0], 0);
	delay(INTERVAL);

	Serial.println("LED 2 on (weak)");
	analogWrite(lockLEDPins[1], 15);
	delay(INTERVAL);
	analogWrite(lockLEDPins[1], 0);
	delay(INTERVAL);

	Serial.println("LED 3 on (weak)");
	analogWrite(lockLEDPins[2], 15);
	delay(INTERVAL);
	analogWrite(lockLEDPins[2], 0);
	delay(INTERVAL);
}

void signalLEDTestsAnalog()
{
	Serial.println("Left LED (weak)");
	analogWrite(signalLED1Pin, 15);
	delay(INTERVAL);
	analogWrite(signalLED1Pin, 0);
	delay(INTERVAL);

	Serial.println("Right LED (weak)");
	analogWrite(signalLED2Pin, 15);
	delay(INTERVAL);
	analogWrite(signalLED2Pin, 0);
	delay(INTERVAL);
}

void motorTest(const uint8_t motor[NREELS][2], uint8_t n)
{
	Serial.print("Motor ");
	Serial.println(n);

	digitalWrite(motor[n][0], LOW);
	digitalWrite(motor[n][1], HIGH);
	delay(INTERVAL);
	digitalWrite(motor[n][0], LOW);
	digitalWrite(motor[n][1], LOW);
	delay(INTERVAL);
	digitalWrite(motor[n][0], HIGH);
	digitalWrite(motor[n][1], LOW);
	delay(INTERVAL);
	digitalWrite(motor[n][0], HIGH);
	digitalWrite(motor[n][1], HIGH);
	delay(INTERVAL);
}

void motorTests()
{
	motorTest(motorOutPins, 0);
	motorTest(motorOutPins, 1);
	motorTest(motorOutPins, 2);
}

// void powerOutputTest(uint8_t port)
// {
// 	digitalWrite(port, HIGH);
// 	delay(INTERVAL);
// 	digitalWrite(port, LOW);
// 	delay(INTERVAL);
// }

// void powerOutputsTests()
// {
// 	char *buf = "\0\0\0";
// 	const uint8_t ports[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 46};

// 	for(size_t i = 0; i < sizeof ports; i++) {
// 		Serial.println("Port");
// 		itoa(ports[i], buf, 10);
// 		Serial.println(buf);
// 		powerOutputTest(ports[i]);
// 	}
// }

void outputTestsSetup()
{
	Serial.begin(BAUDRATE);
	Serial.println("-------------- Output tests");
	ioSetup();
}

void outputTestsLoop()
{
	lockLEDTests();
	// lockLEDTestsAnalog();
	signalLEDTests();
	signalLEDTestsAnalog();
	// motorTests();
}
