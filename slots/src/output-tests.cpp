
#include <Arduino.h>

#include "../slots.h"
#include "./oled-display.h"
#include "./output-tests.h"

OledDisplay oledD;

void outputTestsSetup()
{
	oledD.Setup();
	oledD.PrintS(0, 0, "Output tests");

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

#define INTERVAL 500

void lockLEDTests()
{
	oledD.PrintS(1, 0, "LED 1 on");
	digitalWrite(lockLED[0], HIGH);
	delay(INTERVAL);
	oledD.PrintS(1, 0, "        ");
	digitalWrite(lockLED[0], LOW);
	delay(INTERVAL);

	oledD.PrintS(2, 0, "LED 2 on");
	digitalWrite(lockLED[1], HIGH);
	delay(INTERVAL);
	oledD.PrintS(2, 0, "        ");
	digitalWrite(lockLED[1], LOW);
	delay(INTERVAL);

	oledD.PrintS(3, 0, "LED 3 on");
	digitalWrite(lockLED[2], HIGH);
	delay(INTERVAL);
	oledD.PrintS(3, 0, "        ");
	digitalWrite(lockLED[2], LOW);
	delay(INTERVAL);
}

void redLEDTests()
{
	oledD.PrintS(1, 0, "Right LED");
	digitalWrite(redLED1[0], LOW);
	digitalWrite(redLED1[1], HIGH);
	delay(INTERVAL);
	oledD.PrintS(1, 0, "         ");
	digitalWrite(redLED1[0], LOW);
	digitalWrite(redLED1[1], LOW);
	delay(INTERVAL);

	oledD.PrintS(2, 0, "Left LED");
	digitalWrite(redLED2[0], LOW);
	digitalWrite(redLED2[1], HIGH);
	delay(INTERVAL);
	oledD.PrintS(2, 0, "         ");
	digitalWrite(redLED2[0], LOW);
	digitalWrite(redLED2[1], LOW);
	delay(INTERVAL);
}

void motorTest(const uint8_t motor[2], char *msg)
{
	oledD.PrintS(1, 0, "Motor ");
	oledD.PrintS(1, 6, msg);

	digitalWrite(motor[0], LOW);
	digitalWrite(motor[1], HIGH);
	delay(INTERVAL);
	digitalWrite(motor[0], LOW);
	digitalWrite(motor[1], LOW);
	delay(INTERVAL);
	digitalWrite(motor[0], HIGH);
	digitalWrite(motor[1], LOW);
	delay(INTERVAL);
	digitalWrite(motor[0], HIGH);
	digitalWrite(motor[1], HIGH);
	delay(INTERVAL);
}

void motorTests()
{
	motorTest(motor1Out, "1");
	motorTest(motor2Out, "2");
	motorTest(motor3Out, "3");
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
	oledD.PrintS(1, 5, "  ");

	for(size_t i = 0; i < sizeof ports; i++) {
		oledD.PrintS(1, 0, "Port");
		itoa(ports[i], buf, 10);
		oledD.PrintS(2, 0, buf);
		powerOutputTest(ports[i]);
	}
}
