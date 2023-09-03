
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#pragma once

// ----------------------------------------------------------- Class declaration

/**
 * Implements the lock logic.
 */
class Locks
{
	private:

		uint8_t currentBet;
		uint8_t lastBet = -1;
		uint8_t lastLockedIndex = -1;
		uint8_t lastLocksAllowed = -1;
		uint8_t locksAllowed;

		uint8_t getLockedLocks();
		void calcLocksAllowed();
		void debug(uint8_t index);
		void initLock(uint8_t i);
		void setBlocked(uint8_t index);
		void setLocked(uint8_t index);
		void setStateAsNeeded();
		void setUnlocked(uint8_t index);
		void toggleLock(uint8_t index);

	public:

		void Loop(uint8_t gameBet);
		void Setup();
};

// ------------------------------------------------------------------------- End
