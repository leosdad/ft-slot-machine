
#include <Arduino.h>

#include "../slots.h"
#include "motor-driver-tests.h"
#include "../drivers/motor-driver.h"

MotorDriver m1;

void motorDriverTestsSetup()
{
	Serial.begin(57600);
	m1.Init(motorOutPins[1]);
}

void motorDriverTestsLoop()
{
	Serial.println("CW");
	m1.RotateCW(40);
	delay(2000);

	Serial.println("Coast");
	m1.Coast();
	delay(2000);

	Serial.println("CCW");
	m1.RotateCCW(40);
	delay(2000);

	Serial.println("Brake");
	m1.Brake();
	delay(2000);
}
