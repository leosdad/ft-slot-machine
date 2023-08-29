
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include <ezButton.h>
#include "locks.h"
#include "slots.h"

// ----------------------------------------------------------------------- Types

enum class LockState {
	BLOCKED = 0,  // Not locked; may not be locked by user
	LOCKABLE,	  // Not locked, but may be locked by user
	ACTIVE,		  // Locked; may be unlocked by user
};

const char *StateNames[NREELS] = { "BLOCKED", "LOCKABLE", "ACTIVE" };

struct Lock
{
	uint8_t index;
	LockState state = LockState::BLOCKED;
	ezButton ezLockButton{0};		// ezButton lock button object
};

// ------------------------------------------------------------------- Variables

Lock lock[NREELS];

// ---------------------------------------------------- Private member functions

/**
 * Initiliazes a single lock.
 */
void Locks::initLock(uint8_t i)
{
	lock[i].index = i;
	lock[i].ezLockButton = ezButton(lockButtonPins[i], INPUT_PULLUP);
	lock[i].ezLockButton.setDebounceTime(EZBTNDEBOUNCE);
	lock[i].state = LockState::LOCKABLE;
	pinMode(lockLEDPins[i], OUTPUT);
}

void Locks::setLocked(uint8_t index)
{
	if(lock[index].state != LockState::ACTIVE) {
		lock[index].state = LockState::ACTIVE;
		digitalWrite(lockLEDPins[index], HIGH);
		lastLockedIndex = index;
	}
}

void Locks::setUnlocked(uint8_t index)
{
	if(lock[index].state != LockState::LOCKABLE) {
		digitalWrite(lockLEDPins[index], LOW);
		lock[index].state = LockState::LOCKABLE;
	}
}

void Locks::toggleLock(uint8_t index)
{
	if(lock[index].state == LockState::ACTIVE) {
		setUnlocked(index);
	} else if(lock[index].state == LockState::LOCKABLE) {
		setLocked(index);
	}

	Serial.print("Locked #" + String(index) + " ");
	Serial.println(StateNames[(int)lock[index].state]);
}

// ---------------------------------------------------- Private member functions

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

// ----------------------------------------------------- Public member functions

/**
 * Sets up lock buttons and LEDs.
 */
void Locks::Setup()
{
	for(int i = 0; i < NREELS; i++) {
		initLock(i);
	}

	allowNext = true;
}

void Locks::Loop()
{
	for(int i = 0; i < NREELS; i++) {
		lock[i].ezLockButton.loop();
		if(lock[i].ezLockButton.isPressed()) {
			toggleLock(i);
		}
	}
}

// ------------------------------------------------------------------------- End
