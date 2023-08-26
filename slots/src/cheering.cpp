
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "cheering.h"

// ----------------------------------------------------- Public member functions

void Cheering::Setup()
{
	leftSignal = ezLED(signalLED1Pin);
	rightSignal = ezLED(signalLED2Pin);
	Start();
}

/**
 * Called while reels are stopped
 */
void Cheering::Loop(bool enable, bool cheerALot)
{
	leftSignal.loop();
	rightSignal.loop();

	if(enable) {
		if(cheerALot) {
			if(leftSignal.getState() == LED_IDLE) {
				leftSignal.blinkInPeriod(CHEERALOTMS, CHEERALOTMS, 3000);
				rightSignal.blinkInPeriod(CHEERALOTMS, CHEERALOTMS, 3000);
			}
		} else {
			if(fadeEnabled && leftSignal.getState() == LED_IDLE) {
				if(fadeCycles-- > 0) {
					if(isFadedIn == false) {
						leftSignal.fade(CHEER_MIN, CHEER_MAX, CHEERMS);
						rightSignal.fade(CHEER_MAX, CHEER_MIN, CHEERMS);
						isFadedIn = true;
					} else {
						leftSignal.fade(CHEER_MAX, CHEER_MIN, CHEERMS);
						rightSignal.fade(CHEER_MIN, CHEER_MAX, CHEERMS);
						isFadedIn = false;
					}
				} else {
					rightSignal.fade(CHEER_MAX, CHEER_MIN, CHEERMS);
					leftSignal.fade(CHEER_MAX, CHEER_MIN, CHEERMS);
					fadeEnabled = false;
				}
			}
		}
	} else {
		leftSignal.turnOFF();
		rightSignal.turnOFF();
	}
}

void Cheering::Start()
{
	fadeCycles = 10;
	fadeEnabled = true;
}

void Cheering::Stop()
{
	leftSignal.cancel();
	rightSignal.cancel();
	fadeEnabled = false;
}

// ------------------------------------------------------------------------- End
