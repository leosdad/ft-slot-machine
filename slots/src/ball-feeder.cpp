
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include <Arduino.h>

#include "ball-feeder.h"

// ----------------------------------------------------- Public member functions

// On the Mega, up to 12 servos can be used without interfering with PWM 
// functionality; use of 12 to 23 motors will disable PWM on pins 11 and 12.

void BallFeeder::Attach(uint8_t pin)
{
	servo1.attach(pin);
	servo1.write(0);
}

void BallFeeder::Detach()
{
	servo1.detach();
}

void BallFeeder::Loop()
{
	if(target != current ) {
		if(millis() - previousMillis > Interval) {
			previousMillis = millis();
			if(target < current) {
				current--;
				servo1.write(current);
			} else if(target > current) {
				current++;
				servo1.write(current);
			}
		}
	}
}

void BallFeeder::Feed()
{
	current = 180;
	target = 0;
}

void BallFeeder::Return()
{
	current = 0;
	target = 180;
}

// ------------------------------------------------------------------------- End
