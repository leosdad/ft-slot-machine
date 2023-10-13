
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include <Arduino.h>
#include <arduino-timer.h>
#include "ball-feeder.h"

// ------------------------------------------------------------ Global variables

auto feederTimer = timer_create_default();
extern BallFeeder feeder;

// ------------------------------------------------------------ Global functions

bool returnServo(void *)
{
	feeder.Return();
	return true;
}

// ----------------------------------------------------- Public member functions

// See https://www.arduino.cc/reference/en/libraries/servo/

void BallFeeder::Setup(uint8_t pin)
{
	servo1.attach(pin);
}

void BallFeeder::Detach()
{
	servo1.detach();
}

void BallFeeder::Loop()
{
	feederTimer.tick();

	if(target != current) {
		if(millis() - previousMillis > interval) {
			previousMillis = millis();
			if(target < current) {
				current -= increment;
				servo1.write(current);
			} else if(target > current) {
				current += increment;
				servo1.write(current);
			}
		}
	}
}

void BallFeeder::Feed()
{
	// current = FEEDER_ENDANGLE;
	target = FEEDER_STARTANGLE;
	interval = FEEDER_FWDINTERVAL;
	increment = FEEDER_FWDINCREMENT;
	feederTimer.in(FEEDER_RETURNTIME, returnServo);
}

void BallFeeder::Return()
{
	target = FEEDER_ENDANGLE;
	increment = FEEDER_BACKINCREMENT;
	interval = FEEDER_BACKINTERVAL;
}

// ------------------------------------------------------------------------- End
