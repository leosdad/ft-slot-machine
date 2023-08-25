
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "slots.h"
#include "locks.h"
#include "game.h"

// ------------------------------------------------------------------- Variables

extern Game game;
Lock lock[NREELS];

// ---------------------------------------------------- Private member functions

/**
 * Activates and deactivates the reel lock state according to the current bet.
 */
// void Locks::calculate()
// {
	// if(!allowNext) {
	// 	for(int i = 0; i < NREELS; i++) {
	// 		game->reels[i].lockable = false;
	// 		game->reels[i].locked = false;
	// 	}
	// 	return;
	// }

	// uint8_t currentlyLocked = 0;
	// uint8_t maxLockable = min(2, game->currentBet / 3);

	// for(int i = 0; i < NREELS; i++) {
	// 	if(game->reels[i].locked) {
	// 		currentlyLocked++;
	// 	}
	// }

	// if(currentlyLocked < maxLockable) {

	// 	for(int i = 0; i < NREELS; i++) {
	// 		game->reels[i].lockable = true;
	// 	}

	// } else if(currentlyLocked == maxLockable) {

	// 	for(int i = 0; i < NREELS; i++) {
	// 		if(!game->reels[i].locked) {
	// 			game->reels[i].lockable = false;
	// 			game->reels[i].locked = false;
	// 		}
	// 	}

	// } else {

	// 	// (currentlyLocked > maxLockable)
	// 	for(int i = NREELS - 1; i >= 0;  i--) {
	// 		if(game->reels[i].locked) {
	// 			game->reels[i].locked = false;
	// 			currentlyLocked--;
	// 			if(currentlyLocked <= maxLockable) {
	// 				break;
	// 			}
	// 		}
	// 	}

	// }
// }

/**
 * Lock / unlock logic called once after a spin is completed.
 */
	// if(preventNext) {
	// 	allowNext = false;
	// 	preventNext = false;
	// } else if(game->spinPayoff) {
	// 	allowNext = false;
	// } else {
	// 	allowNext = true;
	// 	for(int i = 0; i < NREELS; i++) {
	// 		if(game->reels[i].locked) {
	// 			allowNext = false;
	// 			break;
	// 		}
	// 	}
	// }

	// if(allowNext) {
	// 	for(int i = 0; i < NREELS; i++) {
	// 		game->reels[i].lockable = true;
	// 		game->reels[i].locked = false;
	// 	}
	// } else {
	// 	for(int i = 0; i < NREELS; i++) {
	// 		game->reels[i].lockable = false;
	// 		game->reels[i].locked = false;
	// 	}
	// }

/**
 * Shows debug information about the lock state.
 */
void Locks::debug(uint8_t currentlyLocked, uint8_t maxLockable)
{
	// Debug

	if((currentlyLocked != lastLocked) ||
		(maxLockable != lastMaxLockable) || 
		(game.currentBet != lastBetValue)) {
		uint8_t allowed = 0;
		for(int i = 0; i < NREELS; i++) {
			if(lock[i].IsAllowed()) {
				allowed++;
			}
		}
		
		Serial.print("Locked: ");
		Serial.print(currentlyLocked);
		Serial.print("  Max: ");
		Serial.print(maxLockable);
		Serial.print("  Bet: ");
		Serial.print(game.currentBet);
		Serial.print("  Allowed: ");
		Serial.println(allowed);

		lastLocked = currentlyLocked;
		lastMaxLockable = maxLockable;
		lastBetValue = game.currentBet;
	};
}

// ----------------------------------------------------- Public member functions

/**
 * Setup lock buttons and LEDs.
 */
void Locks::Setup()
{
	for(int i = 0; i < NREELS; i++) {
		lock[i].Setup(i, lockButtonPins[i], lockLEDPins[i]);
	}
}

void Locks::Loop(bool isSpinning)
{
	// Used only if reels are stopped

	if(isSpinning) {
		for(int i = 0; i < NREELS; i++) {
			lock[i].FrozenLoop();
		}
		return;
	}

	uint8_t currentlyLocked = 0;
	uint8_t maxLockable = min(2, game.currentBet / 3);

	// Calculate total of reels locked

	for(int i = 0; i < NREELS; i++) {
		if(lock[i].Loop(ledState)) {
			currentlyLocked++;
		}
	}

	// Lock logic

	if(currentlyLocked < maxLockable) {

		for(int i = 0; i < NREELS; i++) {
			lock[i].Allow(true);
		}

	} else if(currentlyLocked == maxLockable) {

		for(int i = 0; i < NREELS; i++) {
			if(!lock[i].IsLocked()) {
				lock[i].Allow(false);
			}
		}

	} else {	// (currentlyLocked > maxLockable)

		for(int i = NREELS - 1; i >= 0;  i--) {
			if(lock[i].IsLocked()) {
				lock[i].Unlock();
				currentlyLocked--;
				if(currentlyLocked <= maxLockable) {
					break;
				}
			}
		}

	}

	#if LOCKDEBUGINFO
	debug(currentlyLocked, maxLockable);
	#endif
}

// ------------------------------------------------------------------------- End
