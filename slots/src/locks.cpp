
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
 * Lock / unlock logic called once after each spin is completed.
 */
void Locks::AllowOrBlock(bool allow)
{
	Serial.print(allow);

	allowNext = allow;
	if(allow) {
		allowNext = true;
		for(int i = 0; i < NREELS; i++) {
			if(lock[i].IsLocked()) {
				allowNext = false;
				break;
			}
		}
	} else {
		allowNext = false;
	}

	if(allowNext) {
		for(int i = 0; i < NREELS; i++) {
			lock[i].SetAllowed(true);
			lock[i].SetLocked(false);
		}
	} else {
		for(int i = 0; i < NREELS; i++) {
			lock[i].SetAllowed(false);
			lock[i].SetLocked(false);
		}
	}
}

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

	allowNext = true;
}

void Locks::Loop()
{
	for(int i = 0; i < NREELS; i++) {
		if(lock[i].Loop(ledState)) {
			CalcLocked();
		}
	}
}

/**
 * Activates and deactivates the reel lock state according to the current bet.
 */
void Locks::CalcLocked()
{
	if(!allowNext) {

		for(int i = 0; i < NREELS; i++) {
			lock[i].SetAllowed(false);
			lock[i].SetLocked(false);
		}

	} else {

		uint8_t currentlyLocked = 0;
		uint8_t maxLockable = min(2, game.currentBet / 3);

		for(int i = 0; i < NREELS; i++) {
			if(lock[i].IsLocked()) {
				currentlyLocked++;
			}
		}

		// Lock logic

		if(currentlyLocked < maxLockable) {

			for(int i = 0; i < NREELS; i++) {
				lock[i].SetAllowed(true);
			}

		} else if(currentlyLocked == maxLockable) {

			for(int i = 0; i < NREELS; i++) {
				if(!lock[i].IsLocked()) {
					lock[i].SetAllowed(false);
				}
			}

		} else {	// (currentlyLocked > maxLockable)

			for(int i = NREELS - 1; i >= 0;  i--) {
				if(lock[i].IsLocked()) {
					lock[i].SetLocked(false);
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
}

// ------------------------------------------------------------------------- End
