
#include <Arduino.h>

#include "../../slots.h"
#include "../oled-display-driver.h"
#include "./output-tests.h"

OledDisplayDriver oledDOut;

#define INTERVAL 500

void outputTestsSetup()
{
	oledDOut.Setup();
	oledDOut.PrintS(0, 0, "Output tests");

	pinMode(motorOutPins[0][0], OUTPUT);
	pinMode(motorOutPins[0][1], OUTPUT);
	pinMode(motorOutPins[1][0], OUTPUT);
	pinMode(motorOutPins[1][1], OUTPUT);
	pinMode(motorOutPins[2][0], OUTPUT);
	pinMode(motorOutPins[2][1], OUTPUT);

	pinMode(lockLEDPins[0], OUTPUT);
	pinMode(lockLEDPins[1], OUTPUT);
	pinMode(lockLEDPins[2], OUTPUT);

	pinMode(signalLED1Pin, OUTPUT);
	pinMode(signalLED2Pin, OUTPUT);
}

void lockLEDTests()
{
	oledDOut.PrintS(1, 0, "LED 1 on");
	digitalWrite(lockLEDPins[0], HIGH);
	delay(INTERVAL);
	oledDOut.PrintS(1, 0, "        ");
	digitalWrite(lockLEDPins[0], LOW);
	delay(INTERVAL);

	oledDOut.PrintS(2, 0, "LED 2 on");
	digitalWrite(lockLEDPins[1], HIGH);
	delay(INTERVAL);
	oledDOut.PrintS(2, 0, "        ");
	digitalWrite(lockLEDPins[1], LOW);
	delay(INTERVAL);

	oledDOut.PrintS(3, 0, "LED 3 on");
	digitalWrite(lockLEDPins[2], HIGH);
	delay(INTERVAL);
	oledDOut.PrintS(3, 0, "        ");
	digitalWrite(lockLEDPins[2], LOW);
	delay(INTERVAL);
}

void signalLEDTests()
{
	oledDOut.PrintS(1, 0, "Left LED");
	digitalWrite(signalLED1Pin, HIGH);
	delay(INTERVAL);
	oledDOut.PrintS(1, 0, "         ");
	digitalWrite(signalLED1Pin, LOW);
	delay(INTERVAL);

	oledDOut.PrintS(2, 0, "Right LED");
	digitalWrite(signalLED2Pin, HIGH);
	delay(INTERVAL);
	oledDOut.PrintS(2, 0, "         ");
	digitalWrite(signalLED2Pin, LOW);
	delay(INTERVAL);
}

void lockLEDTestsAnalog()
{
	oledDOut.PrintS(1, 0, "LED 1 on");
	analogWrite(lockLEDPins[0], 15);
	delay(INTERVAL);
	oledDOut.PrintS(1, 0, "        ");
	analogWrite(lockLEDPins[0], 0);
	delay(INTERVAL);

	oledDOut.PrintS(2, 0, "LED 2 on");
	analogWrite(lockLEDPins[1], 15);
	delay(INTERVAL);
	oledDOut.PrintS(2, 0, "        ");
	analogWrite(lockLEDPins[1], 0);
	delay(INTERVAL);

	oledDOut.PrintS(3, 0, "LED 3 on");
	analogWrite(lockLEDPins[2], 15);
	delay(INTERVAL);
	oledDOut.PrintS(3, 0, "        ");
	analogWrite(lockLEDPins[2], 0);
	delay(INTERVAL);
}

void signalLEDTestsAnalog()
{
	oledDOut.PrintS(1, 0, "Left LED");
	analogWrite(signalLED1Pin, 15);
	delay(INTERVAL);
	oledDOut.PrintS(1, 0, "         ");
	analogWrite(signalLED1Pin, 0);
	delay(INTERVAL);

	oledDOut.PrintS(2, 0, "Right LED");
	analogWrite(signalLED2Pin, 15);
	delay(INTERVAL);
	oledDOut.PrintS(2, 0, "         ");
	analogWrite(signalLED2Pin, 0);
	delay(INTERVAL);
}

void motorTest(const uint8_t motor[NREELS][2], uint8_t n)
{
	oledDOut.PrintS(1, 0, "Motor ");
	oledDOut.PrintN(1, 6, n);

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

void powerOutputTest(uint8_t port)
{
	digitalWrite(port, HIGH);
	delay(INTERVAL);
	digitalWrite(port, LOW);
	delay(INTERVAL);
}

void powerOutputsTests()
{
	char *buf = "\0\0\0";
	const uint8_t ports[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 46};
	oledDOut.PrintS(1, 5, "  ");

	for(size_t i = 0; i < sizeof ports; i++) {
		oledDOut.PrintS(1, 0, "Port");
		itoa(ports[i], buf, 10);
		oledDOut.PrintS(2, 0, buf);
		powerOutputTest(ports[i]);
	}
}
