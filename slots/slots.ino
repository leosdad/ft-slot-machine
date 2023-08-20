
// -----------------------------------------------------------------------------

// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "src/slots-main.h"
// #include "src/tests/output-tests.h"
// #include "src/tests/led-matrix-tests.h"

// ----------------------------------------------------------------------- Setup

SlotsMain slotsMain;

void setup()
{
	// outputTestsSetup();
	// ledMatrixTestsSetup();
	slotsMain.Setup();
}

// ------------------------------------------------------------------- Main loop

void loop()
{
	// outputTestsLoop();
	// ledMatrixTestsLoop();
	slotsMain.Loop();
}

// ------------------------------------------------------------------------- End
