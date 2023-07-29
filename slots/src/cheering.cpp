
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
	analogWrite(signalLED1[0], 0);
	analogWrite(signalLED2[0], 0);
}

/**
 * Called while reels are stopped
 */
void Cheering::Loop(Game *game)
{
	if(game->spinPayoff) {
		bool cheerALot = game->spinPayoff / payoffMultiplier > CHEERALOT;
		unsigned long currMs = millis();

		if(currMs - blinkPreviousMs >= (cheerALot ? CHEERALOTMS : CHEERNORMALMS)) {
			blinkPreviousMs = currMs;
			blinkLedState = !blinkLedState;
		}
	
		int value = cheerALot ? 255 : 10;
		analogWrite(signalLED1[1], blinkLedState ? value : 0);
		analogWrite(signalLED2[1], blinkLedState ? 0 : value);
	}
}

void Cheering::Stop()
{
	analogWrite(signalLED1[1], 0);
	analogWrite(signalLED2[1], 0);
}

// ------------------------------------------------------------------------- End
