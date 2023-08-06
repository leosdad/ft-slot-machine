
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "locks.h"
#include "../slots.h"

// ----------------------------------------------------- Public member functions

/**
 * Constructor.
 */
Locks::Locks()
{
	blinkPreviousMs = 0;
	blinkLedState = LOW;
	allowNext = false;
	preventNext = false;
}

/**
 * Called while reels are stopped
 */
void Locks::LoopWhenStopped(Game *game)
{
    unsigned long currMs = millis();

    if(currMs - blinkPreviousMs >= (blinkLedState ? LOCKBLINKON : LOCKBLINKOFF)) {
        blinkPreviousMs = currMs;
        blinkLedState = !blinkLedState;
    }
}

/**
 * Activates and deactivates the reel lock state according to the current bet.
 */
void Locks::CalcLocked(Game *game)
{
	if(!allowNext) {
		for(int i = 0; i < NREELS; i++) {
			game->reels[i].lockable = false;
			game->reels[i].locked = false;
		}
		return;
	}

	uint8_t currentLocked = 0;
	uint8_t maxLockable = min(2, game->currentBet / 3);

	for(int i = 0; i < NREELS; i++) {
		if(game->reels[i].locked) {
			currentLocked++;
		}
	}

	if(currentLocked < maxLockable) {

		for(int i = 0; i < NREELS; i++) {
			game->reels[i].lockable = true;
		}

	} else if(currentLocked == maxLockable) {

		for(int i = 0; i < NREELS; i++) {
			if(!game->reels[i].locked) {
				game->reels[i].lockable = false;
				game->reels[i].locked = false;
			}
		}

	} else {

		// (currentLocked > maxLockable)
		for(int i = NREELS - 1; i >= 0;  i--) {
			if(game->reels[i].locked) {
				game->reels[i].locked = false;
				currentLocked--;
				if(currentLocked <= maxLockable) {
					break;
				}
			}
		}

	}
}

/**
 * Determines if lock will be allowed for the next spin.
 */
void Locks::AllowNext(bool prevent)
{
	preventNext = prevent;
}

/**
 * Lock / unlock logic called once after a spin is completed.
 */
void Locks::LockUnlock(Game *game)
{
	if(preventNext) {
		allowNext = false;
		preventNext = false;
	} else if(game->spinPayoff) {
		allowNext = false;
	} else {
		allowNext = true;
		for(int i = 0; i < NREELS; i++) {
			if(game->reels[i].locked) {
				allowNext = false;
				break;
			}
		}
	}

	if(allowNext) {
		for(int i = 0; i < NREELS; i++) {
			game->reels[i].lockable = true;
			game->reels[i].locked = false;
		}
	} else {
		for(int i = 0; i < NREELS; i++) {
			game->reels[i].lockable = false;
			game->reels[i].locked = false;
		}
	}
}

// ------------------------------------------------------------------------- End
