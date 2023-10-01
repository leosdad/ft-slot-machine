
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
		Serial.println("DFPlayer mini player OK.");
	} else {
		Serial.println("Error initializing DFPlayer mini player.");
	}
}

// ------------------------------------------------------------------- Main loop

void soundTestsLoop()
{
	for(uint8_t i = 1; i <= 10; i++) {
		Serial.println("Playing sound " + String(i));
		_sound.Play(i);
		delay(1000);
	}
	delay(200);
}

// ------------------------------------------------------------------------- End
