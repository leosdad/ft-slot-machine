
#include <Arduino.h>

#include "../oled-display.h"
#include "input-tests.h"
#include "output-tests.h"
#include "servo-tests.h"
#include "dfplayer-tests.h"
#include "seven-seg-tests.h"

void testsSetup()
{
	oledSetup();
	oledPrint(0, 0, "Start...");

	// ----------------

	// inputTestsSetup();
	outputTestsSetup();
	// servoTestsSetup();
	// dfPlayerTestsSetup();
	// sevenSegTest();
}

void testsLoop()
{
	// Input tests
	// pushButtonTests();	// OK
	// encoderTests();	// OK
	// sensorTests();	// OK
	// toggleTests();	// OK

	// Output tests
	// powerOutputsTests();	// OK
	// motorTests();	// OK
	lockLEDTests();	// OK
	redLEDTests();	// OK

	// Servo tests
	// servoTests();	// OK

	// DFPlayer Tests
	// dfPlayerTests();	// OK

}
