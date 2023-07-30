
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef BALL_FEEDER_H
#define BALL_FEEDER_H

// -------------------------------------------------------------------- Includes

#include <Servo.h>

// ----------------------------------------------------------- Class declaration

class BallFeeder
{
	private:
		Servo servo1;
		uint32_t previousMillis = 0;
		uint16_t current = 0;		// current angle
		uint16_t target = 0;		// target angle
		uint8_t step = 1;			// delay time

	public:
		uint8_t Interval = 20;		// delay time
		void Attach(uint8_t pin);
		void Detach();
		void Loop();
		void Feed();
		void Return();
};

// ------------------------------------------------------------------------- End

#endif // BALL_FEEDER_H
