
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "cheering.h"
#include "payoffs.h"

// ----------------------------------------------------- Public member functions

Cheering::Cheering()
{
	blinkPreviousMs = 0;
	blinkLedState = LOW;
	analogWrite(signalLED1, 0);
	analogWrite(signalLED2, 0);
}

/**
 * Called while reels are stopped
 */
void Cheering::Loop(Game *game)
{
	if(game->spinPayoff) {
		unsigned long currMs = millis();

		if(currMs - blinkPreviousMs >= (game->newBall ? CHEERALOTMS : CHEERNORMALMS)) {
			blinkPreviousMs = currMs;
			blinkLedState = !blinkLedState;
		}
	
		int value = game->newBall ? 255 : 10;
		analogWrite(signalLED1, blinkLedState ? value : 0);
		analogWrite(signalLED2, blinkLedState ? 0 : value);
	}
}

void Cheering::Stop()
{
	analogWrite(signalLED1, 0);
	analogWrite(signalLED2, 0);
}

// ------------------------------------------------------------------------- End
