
// -----------------------------------------------------------------------------

// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "src/slots-main.h"
// #include "src/slots.h"

// ----------------------------------------------------------------------- Setup

SlotsMain slotsMain;

void setup()
{
	// pinMode(increaseBetPin, INPUT_PULLUP);
	// pinMode(decreaseBetPin, INPUT_PULLUP);

	// for(int i = 0; i < NREELS; i++) {
	// 	pinMode(lockLEDPins[i], OUTPUT);
	// 	pinMode(lockButtonPins[i], INPUT_PULLUP);
	// }

	slotsMain.Setup();
}

// ------------------------------------------------------------------- Main loop

void loop()
{
	slotsMain.Loop();
	// for(int i = 0; i < NREELS; i++) {
	// 	digitalWrite(lockLEDPins[i], !digitalRead(lockButtonPins[i]));
	// }
}

// ------------------------------------------------------------------------- End
