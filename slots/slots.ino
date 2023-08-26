
// -----------------------------------------------------------------------------

// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "src/slots-main.h"
// #include "src/tests/led-matrix-tests.h"
// #include "src/tests/output-tests.h"
// #include "src/tests/motor-driver-tests.h"

// ----------------------------------------------------------------------- Setup

SlotsMain slotsMain;

void setup()
{
	slotsMain.Setup();
	// ledMatrixTestsSetup();
	// outputTestsSetup();
	// motorDriverTestsSetup();
}

// ------------------------------------------------------------------- Main loop

void loop()
{
	slotsMain.Loop();
	// ledMatrixTestsLoop();
	// outputTestsLoop();
	// motorDriverTestsLoop();
}

// ------------------------------------------------------------------------- End
