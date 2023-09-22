
// -----------------------------------------------------------------------------

// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include <Arduino.h>
#include "../drivers/sound.h"

Sound _sound;

// ----------------------------------------------------------------------- Setup

void soundTestsSetup()
{
	if(_sound.Setup(10)) {
		_sound.Play(1);
		Serial.println("DFPlayer mini player OK.");
	} else {
		Serial.println("Error initializing DFPlayer mini player.");
	}
}

// ------------------------------------------------------------------- Main loop

void soundTestsLoop()
{
}

// ------------------------------------------------------------------------- End
