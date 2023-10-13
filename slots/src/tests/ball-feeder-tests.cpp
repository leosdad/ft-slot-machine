
// -----------------------------------------------------------------------------

// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include <Arduino.h>
#include <arduino-timer.h>
#include "../ball-feeder.h"

// ------------------------------------------------------------------- Variables

BallFeeder _feeder;
static const uint8_t servoPin = 44;
auto _feedTimer = timer_create_default();

// ------------------------------------------------------------ Global functions

bool _returnServo(void *);
bool _moveServo(void *);

bool _returnServo(void *)
{
	_feeder.Return();
	Serial.println("Return");
	_feedTimer.in(4000, _moveServo);
	return true;
}

bool _moveServo(void *)
{
	_feeder.Feed();
	Serial.println("Feed");
	_feedTimer.in(1000, _returnServo);
	return true;
}

// ----------------------------------------------------------------------- Setup

void ballFeederTestsSetup()
{
	_feeder.Setup(servoPin);
	_feedTimer.in(4000, _moveServo);
	Serial.println("Servo test");
}

// ------------------------------------------------------------------- Main loop

void ballFeederTestsLoop()
{
	_feedTimer.tick();
	_feeder.Loop();
}

// ------------------------------------------------------------------------- End
