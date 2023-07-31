
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "locks.h"

// ----------------------------------------------------- Public member functions

// TODO: Use analogWrite()

Locks::Locks()
{
	blinkPreviousMs = 0;
	blinkLedState = LOW;
	allowNext = false;
}

/**
 * Called while reels are stopped
 */
void Locks::Loop(Game *game)
{
	// Timer for blinking reel lock LEDs
	unsigned long currMs = millis();

	if(currMs - blinkPreviousMs >= LOCKBLINKMS) {
		blinkPreviousMs = currMs;
		blinkLedState = !blinkLedState;
	}

	// TODO: does not need to be called all the time
	CalcLock(game);
}

void Locks::AllowNext(NextState state)
{
	if(state == NextState::ALLOWED) {
		allowNext = true;
	} else if(state == NextState::FORBIDDEN) {
		allowNext = false;
	} else {	// AUTO
		if(!allowNext) {
			allowNext = true;
		}
	}
}

void Locks::CalcLock(Game *game)
{
	// TODO: this logic does not need to be called from the loop; only
	// after a lock button or the bet incr/decr are pressed

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

	} else {	// (currentLocked > maxLockable)

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
 * Lock / unlock logic after a spin is completed.
 */
void Locks::LockUnlock(Game *game)
{
	for(int i = 0; i < NREELS; i++) {
		if(game->reels[i].locked) {
			allowNext = false;
			break;
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
