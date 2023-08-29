
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include <arduino-timer.h>
#include <ezButton.h>
#include "locks.h"
#include "slots.h"

// ----------------------------------------------------------------------- Types

/**
 * Lock states enumeration.
 */
enum class LockState {
	BLOCKED = 0,  // Not locked; Pressing the respective button does nothing.
	LOCKABLE,	  // Not locked, but may be locked by pressing the respective button.
	ACTIVE,		  // Locked; may be unlocked by pressing the respective button.
};

/**
 * Declares a single lock object.
 */
struct Lock
{
	uint8_t index;
	LockState state = LockState::BLOCKED;
	ezButton ezLockButton{0};		// ezButton lock button object
};

// ------------------------------------------------------------ Global variables

Lock lock[NREELS];
auto lockBlink = timer_create_default();
bool lockBlinkState = false;

#if LOCKDEBUGINFO
const static char *StateNames[NREELS] = { "Blocked", "Lockable", "Active" };
#endif

// ------------------------------------------------------------ Global functions

/**
 * Called by the lock blink timer.
 */
bool lockBlinkCallback(void *)
{
	lockBlinkState = !lockBlinkState;
	return true;
}

/**
 * Turns the requested lock LED off.
 */
void ledOff(uint8_t index)
{
	pinMode(lockLEDPins[index], OUTPUT);
	digitalWrite(lockLEDPins[index], LOW);
}

/**
 * Turns the requested lock LED on.
 */
void ledOn(uint8_t index)
{
	pinMode(lockLEDPins[index], OUTPUT);
	digitalWrite(lockLEDPins[index], HIGH);
}

/**
 * Lights up the requested lock LED faintly using a little trick with the
 * internall pull-up.
 */
void ledFaint(uint8_t index)
{
	digitalWrite(lockLEDPins[index], LOW);
	pinMode(lockLEDPins[index], INPUT_PULLUP);
}

// ---------------------------------------------------- Private member functions

#if LOCKDEBUGINFO
/**
 * Outputs debug information to the serial port.
 */
void Locks::debug(uint8_t index)
{
	Serial.println("---------------------------------");
	if(index < NREELS ) {
		Serial.print("Lock #" + String(index) + " set to ");
		Serial.println(StateNames[(int)lock[index].state]);
	}
	Serial.println("Current bet: " + String(currentBet));
	Serial.println("Locks allowed: " + String(locksAllowed));
	Serial.println("Total locks: " + String(getLockedLocks()));
	Serial.println("Last locked: " + String(lastLockedIndex));
}
#endif

/**
 * Initializes a single lock.
 */
void Locks::initLock(uint8_t i)
{
	lock[i].index = i;
	lock[i].ezLockButton = ezButton(lockButtonPins[i], INPUT_PULLUP);
	lock[i].ezLockButton.setDebounceTime(EZBTNDEBOUNCE);
	pinMode(lockLEDPins[i], OUTPUT);
	setUnlocked(i);
}

/**
 * @returns The number of locked locks.
 */
uint8_t Locks::getLockedLocks()
{
	uint8_t count = 0;
	for(int i = 0; i < NREELS; i++) {
		if(lock[i].state == LockState::ACTIVE) {
			count++;
		}
	}
	return count;
}

/**
 * Sets the required lock to active state.
 */
void Locks::setLocked(uint8_t index)
{
	lock[index].state = LockState::ACTIVE;
	ledOn(index);
	lastLockedIndex = index;
}

/**
 * Sets the required lock to lockable state.
 */
void Locks::setUnlocked(uint8_t index)
{
	lock[index].state = LockState::LOCKABLE;
}

void Locks::setBlocked(uint8_t index)
{
	lock[index].state = LockState::BLOCKED;
	ledOff(index);
}

/**
 * Called whenever a lock button is pressed.
 */
void Locks::toggleLock(uint8_t index)
{
	LockState prevLockState = lock[index].state;

	if(lock[index].state == LockState::ACTIVE) {
		setUnlocked(index);
	} else if(lock[index].state == LockState::LOCKABLE) {
		setLocked(index);
	}

	#if LOCKDEBUGINFO
	if(lock[index].state != prevLockState) {
		debug(index);
	}
	#endif
}

/**
 * Stores the number of lock currently allowed in the `locksAllowed` variable.
 */
void Locks::calcLocksAllowed()
{
	if(currentBet == 0) {
		#if LOCKDEBUGINFO
		lastBet = 0;
		#endif
		locksAllowed = 0;
		lastLockedIndex = -1;
	} else {
		locksAllowed = min((NREELS - 1), currentBet / NREELS);
	}

	#if LOCKDEBUGINFO
		if((locksAllowed != lastLocksAllowed) || (lastBet != currentBet)) {
		debug(-1);
		}
		lastLocksAllowed = locksAllowed;
	#endif

}

/**
 * Enable, disable or block locks according to the number of locks allowed and
 * the number of locked locks.
 */
void Locks::setStateAsNeeded()
{
	uint8_t lockedLocks = getLockedLocks();

	// Sets lastLockedIndex if needed. This works because NREELS is only 3

	if(lockedLocks == 1) {
		for(int i = 0; i < NREELS; i++) {
			if(lock[i].state == LockState::ACTIVE) {
				lastLockedIndex = i;
				break;
			}
		}
	}

	// Switch according to the number of locks allowed and locked locks

	if(locksAllowed > lockedLocks) {

		// Enable all non-active locks

		for(int i = 0; i < NREELS; i++) {
			if(lock[i].state != LockState::ACTIVE) {
				setUnlocked(i);
			}
		}

	} else if(locksAllowed == lockedLocks) {

		// Maximum reached, so block all non-active locks

		for(int i = 0; i < NREELS; i++) {
			if(lock[i].state != LockState::ACTIVE) {
				setBlocked(i);
			}
		}

	} else {	// locksAllowed < lockedLocks

		// Unlock the last locked lock

		if(lastLockedIndex >= 0 && lastLockedIndex <= NREELS) {
			lock[lastLockedIndex].state = LockState::LOCKABLE;
			lastLockedIndex = -1;
		}
	}
}

// ----------------------------------------------------- Public member functions

/**
 * Sets up lock buttons and LEDs.
 */
void Locks::Setup()
{
	for(int i = 0; i < NREELS; i++) {
		initLock(i);
	}
	lockBlink.every(LOCKBLINK, lockBlinkCallback);
}

/**
 * Must be called from the main loop.
 */
void Locks::Loop(uint8_t gameBet)
{
	currentBet = gameBet;

	#if LOCKDEBUGINFO
	if(gameBet != lastBet) {
		lastBet = gameBet;
	}
	#endif

	for(int i = 0; i < NREELS; i++) {
		lock[i].ezLockButton.loop();
		if(lock[i].ezLockButton.isPressed()) {
			toggleLock(i);
		}
		if(lock[i].state == LockState::LOCKABLE) {
			if(lockBlinkState) {
				ledFaint(i);
			} else {
				ledOff(i);
			}
		}
	}

	calcLocksAllowed();
	setStateAsNeeded();

	lockBlink.tick();
}

// ------------------------------------------------------------------------- End
