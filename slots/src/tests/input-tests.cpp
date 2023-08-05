
#include <Arduino.h>
#include "../../slots.h"
#include "../oled-display-driver.h"
#include "./input-tests.h"

OledDisplayDriver oledDIn;

void inputTestsSetup()
{
	oledDIn.Setup();
	oledDIn.PrintS(0, 0, "Input tests");

	pinMode(decreaseBetPin, INPUT_PULLUP);
	pinMode(increaseBetPin, INPUT_PULLUP);
	pinMode(leverButtonPin, INPUT_PULLUP);

	pinMode(lockButtonPins[0], INPUT_PULLUP);
	pinMode(lockButtonPins[1], INPUT_PULLUP);
	pinMode(lockButtonPins[2], INPUT_PULLUP);

	pinMode(encoderPins[0], INPUT_PULLUP);
	pinMode(encoderPins[1], INPUT_PULLUP);
	pinMode(encoderPins[2], INPUT_PULLUP);
}

void pushButtonTests()
{
	if(!digitalRead(decreaseBetPin)) {
		oledDIn.PrintS(1, 0, "decrease");
	} else {
		oledDIn.PrintS(1, 0, "                ");
	}
	if(!digitalRead(increaseBetPin)) {
		oledDIn.PrintS(2, 0, "increase");
	} else {
		oledDIn.PrintS(2, 0, "                ");
	}
	if(!digitalRead(leverButtonPin)) {
		oledDIn.PrintS(3, 0, "start");
	} else {
		oledDIn.PrintS(3, 0, "                ");
	}
}

void encoderTests()
{
	if(digitalRead(encoderPins[0])) {
		oledDIn.PrintS(1, 0, "enc1");
	} else {
		oledDIn.PrintS(1, 0, "    ");
	}
	if(digitalRead(encoderPins[1])) {
		oledDIn.PrintS(2, 0, "enc2");
	} else {
		oledDIn.PrintS(2, 0, "    ");
	}
	if(digitalRead(encoderPins[2])) {
		oledDIn.PrintS(3, 0, "enc3");
	} else {
		oledDIn.PrintS(3, 0, "    ");
	}
}

void homeSensorTests()
{
	if(digitalRead(homeSensorPins[0])) {
		oledDIn.PrintS(1, 0, "sens1");
	} else {
		oledDIn.PrintS(1, 0, "     ");
	}
	if(digitalRead(homeSensorPins[1])) {
		oledDIn.PrintS(2, 0, "sens2");
	} else {
		oledDIn.PrintS(2, 0, "     ");
	}
	if(digitalRead(homeSensorPins[2])) {
		oledDIn.PrintS(3, 0, "sens3");
	} else {
		oledDIn.PrintS(3, 0, "     ");
	}
}

void lockButtonTests()
{
	if(!digitalRead(lockButtonPins[0])) {
		oledDIn.PrintS(1, 0, "tog1");
	} else {
		oledDIn.PrintS(1, 0, "    ");
	}
	if(!digitalRead(lockButtonPins[1])) {
		oledDIn.PrintS(2, 0, "tog2");
	} else {
		oledDIn.PrintS(2, 0, "    ");
	}
	if(!digitalRead(lockButtonPins[2])) {
		oledDIn.PrintS(3, 0, "tog3");
	} else {
		oledDIn.PrintS(3, 0, "    ");
	}
}

