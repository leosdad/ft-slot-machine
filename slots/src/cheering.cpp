
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "cheering.h"

// ----------------------------------------------------- Public member functions

void Cheering::Setup()
{
	leftSignal = ezLED(signalLED1Pin);
	rightSignal = ezLED(signalLED2Pin);
	Start(CheerLevel::NONE);
}

/**
 * Called while reels are stopped
 */
void Cheering::Loop(bool enable)
{
	leftSignal.loop();
	rightSignal.loop();

	if(enable) {
		if(leftSignal.getState() == LED_IDLE && cheerLevel > CheerLevel::NONE) {
			switch(cheerLevel) {
				case CheerLevel::NONE:
					break;

				case CheerLevel::DRAW:
					if(active) {
						if(fadeCycles++ < CHEER_DRAW_RPT) {
							if(firstCycle) {
								leftSignal.fade(CHEER_MIN_BRT, CHEER_DRAW_BRT, CHEER_DRAW_MS);
								rightSignal.fade(CHEER_MIN_BRT, CHEER_DRAW_BRT, CHEER_DRAW_MS);
								firstCycle = false;
							} else {
								leftSignal.fade(CHEER_DRAW_BRT, CHEER_MIN_BRT, CHEER_DRAW_MS);
								rightSignal.fade(CHEER_DRAW_BRT, CHEER_MIN_BRT, CHEER_DRAW_MS);
								firstCycle = true;
							}
						} else {
							leftSignal.fade(CHEER_DRAW_BRT, 0, CHEER_OUT_MS);
							rightSignal.fade(CHEER_DRAW_BRT, 0, CHEER_OUT_MS);
							active = false;
						}
					}
					break;

				case CheerLevel::WIN:
					if(active) {
						if(fadeCycles++ < CHEER_WIN_RPT) {
							if(firstCycle) {
								leftSignal.fade(CHEER_MIN_BRT, CHEER_WIN_BRT, CHEER_WIN_MS);
								rightSignal.fade(CHEER_WIN_BRT, CHEER_MIN_BRT, CHEER_WIN_MS);
								firstCycle = false;
							} else {
								leftSignal.fade(CHEER_WIN_BRT, CHEER_MIN_BRT, CHEER_WIN_MS);
								rightSignal.fade(CHEER_MIN_BRT, CHEER_WIN_BRT, CHEER_WIN_MS);
								firstCycle = true;
							}
						} else {
							leftSignal.fade(CHEER_WIN_BRT, 0, CHEER_OUT_MS);
							rightSignal.fade(CHEER_WIN_BRT, 0, CHEER_OUT_MS);
							active = false;
						}
					}
					break;

				case CheerLevel::BIG_WIN:
					if(active) {
						if(fadeCycles++ < CHEER_LOT_RPT) {
							leftSignal.blinkInPeriod(CHEER_LOT_MS, CHEER_LOT_MS, CHEER_LOT_ON);
							rightSignal.blinkInPeriod(CHEER_LOT_MS, CHEER_LOT_MS, CHEER_LOT_ON);
						} else {
							leftSignal.fade(CHEER_WIN_BRT, 0, CHEER_OUT_MS);
							rightSignal.fade(CHEER_WIN_BRT, 0, CHEER_OUT_MS);
							active = false;
						}
					}
					break;
			}
		}
	} else {
		leftSignal.turnOFF();
		rightSignal.turnOFF();
	}
}

void Cheering::Start(CheerLevel level)
{
	cheerLevel = level;
	fadeCycles = 0;
	active = true;
}

void Cheering::Stop()
{
	leftSignal.cancel();
	rightSignal.cancel();
	active = false;
	firstCycle = true;
}

// ------------------------------------------------------------------------- End
