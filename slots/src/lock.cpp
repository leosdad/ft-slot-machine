
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
	ledPin = lockLEDPinNumber;
	ezLockButton = ezButton(lockButtonPinNumber, INPUT_PULLUP);
	ezLockButton.setDebounceTime(EZBTNDEBOUNCE);
	pwm.Setup(ledPin, LOCKPWM);
}

/**
 * Maybe too expensive justo to keep a LED on or off, but other options are
 * surprisingly hard to make right and debug.
 */
void Lock::FrozenLoop()
{
	digitalWrite(ledPin, locked);
}

/**
 * Monitors the lock button, changes lock state and LED accordingly.
 * Should be called only while reels are stopped.
 * @returns `true` if the lock is locked.
 */
bool Lock::Loop(bool ledOn)
{
	ezLockButton.loop();
	pwm.Loop();

	if(ezLockButton.isPressed()) {
		if(lockAllowed) {
			locked = !locked;
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

	return locked;
}

/**
 * Sets the lock allowed flag value.
 * @returns `true` if the lock is locked.
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

/**
 * Unlocks the lock.
 */
void Lock::Unlock()
{
	locked = false;
}

// ------------------------------------------------------------------------- End
