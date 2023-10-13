
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#pragma once

// -------------------------------------------------------------------- Includes

#include <Servo.h>

// --------------------------------------------------------------------- Defines

#define FEEDER_FWDINTERVAL		1		// Forward delay
#define FEEDER_FWDINCREMENT		3		// Forward increment
#define FEEDER_BACKINTERVAL		5		// Backwards delay
#define FEEDER_BACKINCREMENT	2		// Backwards delay
#define FEEDER_STARTANGLE		140		// Start angle
#define FEEDER_ENDANGLE			20		// End angle
#define FEEDER_RETURNTIME		600		// Return time, in ms

// ----------------------------------------------------------- Class declaration

class BallFeeder
{
	private:
		Servo servo1;
		uint32_t previousMillis = 0;
		uint8_t interval;
		uint8_t increment;
		uint16_t current = 0;	// Current angle
		uint16_t target;		// Target angle

	public:
		void Setup(uint8_t pin);
		void Detach();
		void Loop();
		void Feed();
		void Return();
};

// ------------------------------------------------------------------------- End
