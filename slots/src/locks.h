
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
		bool lastAllowLocks = -1;

		uint8_t getLockedLocks();
		void calcLocksAllowed();
		void initLock(uint8_t i);
		void setBlocked(uint8_t index);
		void setLocked(uint8_t index);
		uint8_t setStateAsNeeded();
		void setUnlocked(uint8_t index);
		void toggleLock(uint8_t index);

		#if LOCKDEBUGINFO
		void debug(uint8_t index);
		#endif

	public:

		bool IsLocked(uint8_t index);
		uint8_t Loop(bool enable, bool allowNext, uint8_t gameBet);
		void Reset();
		void Setup();
};

// ------------------------------------------------------------------------- End
