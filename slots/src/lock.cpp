
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "slots.h"
#include "lock.h"
#include "game.h"

extern Game game;

// ----------------------------------------------------- Public member functions

/**
 * Setup a single lock.
 */
void Lock::Setup(uint8_t lockButtonPinNumber, uint8_t lockLEDPinNumber)
{
	ezLockButton = ezButton(lockButtonPinNumber, INPUT_PULLUP);
	ezLockButton.setDebounceTime(EZBTNDEBOUNCE);
	ezLockLED = ezLED(lockLEDPinNumber);
}

/**
 * Monitors the lock button, changes lock state and LED accordingly.
 * Returns `true` if the lock is locked.
 * Should be called only while reels are stopped.
 */
bool Lock::Loop(bool ledOn)
{
	ezLockButton.loop();
	ezLockLED.loop();

	if(ezLockButton.isPressed()) {
		if(lockAllowed) {
			locked = !locked;
		}
	}

	if(locked) {
		ezLockLED.turnON();
	} else {
		if(lockAllowed) {
			if(ledOn) {
				ezLockLED.blink(1, 5);	// PWM of sorts
			} else {
				ezLockLED.turnOFF();
			}
		} else {
			ezLockLED.turnOFF();
		}
	}

	return locked;
}

/**
 * Sets the lock allowed flag value.
 * Returns `true` if the lock is locked.
 */
bool Lock::Allow(bool allow)
{
	lockAllowed = allow;
	if(!lockAllowed) {
		locked = false;
	}
	return locked;
}

/**
 * Returns the locked state.
 */
bool Lock::Locked()
{
	return locked;
}

/**
 * Returns the allowed state.
 */
bool Lock::Allowed()
{
	return lockAllowed;
}

/**
 * Unlocks the lock.
 */
void Lock::Unlock()
{
	locked = false;
}

// ------------------------------------------------------------------------- End
