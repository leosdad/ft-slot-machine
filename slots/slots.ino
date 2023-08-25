
// -----------------------------------------------------------------------------

// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "src/slots-main.h"
// #include "src/tests/led-matrix-tests.h"

// ----------------------------------------------------------------------- Setup

SlotsMain slotsMain;

void setup()
{
	// ledMatrixTestsSetup();
	slotsMain.Setup();
}

// ------------------------------------------------------------------- Main loop

void loop()
{
	// ledMatrixTestsLoop();
	slotsMain.Loop();
}

// ------------------------------------------------------------------------- End
