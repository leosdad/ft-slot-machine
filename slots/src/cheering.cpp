
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "cheering.h"

// ----------------------------------------------------- Public member functions

void Cheering::Start()
{
	leftSignal = ezLED(signalLED1Pin);
	rightSignal = ezLED(signalLED2Pin);
}

/**
 * Called while reels are stopped
 */
void Cheering::Loop(bool enable, bool cheerALot)
{
	if(enable) {

		leftSignal.loop();
		rightSignal.loop();

		if(cheerALot) {
			if(leftSignal.getState() == LED_IDLE) {
				leftSignal.blink(CHEERALOTMS, CHEERALOTMS);
				rightSignal.blink(CHEERALOTMS, CHEERALOTMS);
			}
		} else {
			if(leftSignal.getState() == LED_IDLE) {
				if(isFadedIn == false) {
					leftSignal.fade(CHEER_MIN, CHEER_MAX, CHEERMS);
					rightSignal.fade(CHEER_MAX, CHEER_MIN, CHEERMS);
					isFadedIn = true;
				} else {
					leftSignal.fade(CHEER_MAX, CHEER_MIN, CHEERMS);
					rightSignal.fade(CHEER_MIN, CHEER_MAX, CHEERMS);
					isFadedIn = false;
				}
			}
		}
	} else {
		leftSignal.turnOFF();
		rightSignal.turnOFF();
	}
}

void Cheering::Stop()
{
	leftSignal.cancel();
	rightSignal.cancel();
}

// ------------------------------------------------------------------------- End
