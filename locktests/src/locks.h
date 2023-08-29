
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#pragma once

// ----------------------------------------------------------- Class declaration

class Locks
{
	private:

		bool allowNext;
		uint8_t lastLockedIndex = -1;

		void initLock(uint8_t i);
		void setLocked(uint8_t index);
		void setUnlocked(uint8_t index);
		void toggleLock(uint8_t index);

		uint8_t getLockedLocks();

	public:

		// void ForceCalc();
		void Loop();
		void Setup();
};

// ------------------------------------------------------------------------- End
