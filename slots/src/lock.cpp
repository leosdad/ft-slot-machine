
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
void Lock::Setup(uint8_t lockIndex, uint8_t lockButtonPinNumber, uint8_t lockLEDPinNumber)
{
	index = lockIndex;
	locked = false;
	lockAllowed = false;
	ledPin = lockLEDPinNumber;
	ezLockButton = ezButton(lockButtonPinNumber, INPUT_PULLUP);
	ezLockButton.setDebounceTime(EZBTNDEBOUNCE);
	pwm.Setup(ledPin, LOCKPWM);
}

/**
 * Monitors the lock button, changes lock state and LED accordingly.
 * Should be called only while reels are stopped.
 * @returns `true` if the lock state has changed.
 */
bool Lock::Loop(bool ledOn)
{
	ezLockButton.loop();
	pwm.Loop();

	bool changed = false;

	if(ezLockButton.isPressed()) {
		if(lockAllowed) {
			locked = !locked;
			changed = true;
		}
	}

	if(locked) {
		pwm.TurnOn();
	} else {
		if(lockAllowed) {
			pwm.SetValue(ledOn ? LOCKPWM : 0);
		} else {
			pwm.TurnOff();
		}
	}

	return changed;
}

/**
 * Locks or unlocks the lock.
 */
void Lock::SetLocked(bool lock)
{
	locked = lock;
}

/**
 * Sets the lock allowed flag value.
 * @returns `true` if the lock is locked.
 */
void Lock::SetAllowed(bool allow)
{
	lockAllowed = allow;
}

/**
 * @returns The locked state.
 */
bool Lock::IsLocked()
{
	return locked;
}

/**
 * @returns The allowed state.
 */
bool Lock::IsAllowed()
{
	return lockAllowed;
}

// ------------------------------------------------------------------------- End
