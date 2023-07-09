
#include <Arduino.h>
#include "../../slots.h"
#include "../oled-display.h"

void inputTestsSetup()
{
	pinMode(decreaseBetPin, INPUT_PULLUP);
	pinMode(increaseBetPin, INPUT_PULLUP);
	pinMode(leverButtonPin, INPUT_PULLUP);

	pinMode(toggle[0], INPUT_PULLUP);
	pinMode(toggle[1], INPUT_PULLUP);
	pinMode(toggle[2], INPUT_PULLUP);

	pinMode(encoder[0], INPUT_PULLUP);
	pinMode(encoder[1], INPUT_PULLUP);
	pinMode(encoder[2], INPUT_PULLUP);

	oledPrint(0, 0, "Input tests");
}

void pushButtonTests()
{
	if(!digitalRead(decreaseBetPin)) {
		oledPrint(1, 0, "decrease");
	} else {
		oledClearRow(1);
	}
	if(!digitalRead(increaseBetPin)) {
		oledPrint(2, 0, "increase");
	} else {
		oledClearRow(2);
	}
	if(!digitalRead(leverButtonPin)) {
		oledPrint(3, 0, "start");
	} else {
		oledClearRow(3);
	}
}

void encoderTests()
{
	if(digitalRead(encoder[0])) {
		oledPrint(1, 0, "enc1");
	} else {
		oledPrint(1, 0, "    ");
	}
	if(digitalRead(encoder[1])) {
		oledPrint(2, 0, "enc2");
	} else {
		oledPrint(2, 0, "    ");
	}
	if(digitalRead(encoder[2])) {
		oledPrint(3, 0, "enc3");
	} else {
		oledPrint(3, 0, "    ");
	}
}

void sensorTests()
{
	if(digitalRead(positionSensor[0])) {
		oledPrint(1, 0, "sens1");
	} else {
		oledPrint(1, 0, "     ");
	}
	if(digitalRead(positionSensor[1])) {
		oledPrint(2, 0, "sens2");
	} else {
		oledPrint(2, 0, "     ");
	}
	if(digitalRead(positionSensor[2])) {
		oledPrint(3, 0, "sens3");
	} else {
		oledPrint(3, 0, "     ");
	}
}

void toggleTests()
{
	if(!digitalRead(toggle[0])) {
		oledPrint(1, 0, "tog1");
	} else {
		oledPrint(1, 0, "    ");
	}
	if(!digitalRead(toggle[1])) {
		oledPrint(2, 0, "tog2");
	} else {
		oledPrint(2, 0, "    ");
	}
	if(!digitalRead(toggle[2])) {
		oledPrint(3, 0, "tog3");
	} else {
		oledPrint(3, 0, "    ");
	}
}

