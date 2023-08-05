
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef LOCKS_H
#define LOCKS_H

// -------------------------------------------------------------------- Includes

#include "../slots.h"
#include "game.h"

// ----------------------------------------------------------- Class declaration

class Locks
{
	private:

		unsigned long blinkPreviousMs;
		bool allowNext;
		bool preventNext;

	public:

		int blinkLedState;

		Locks();

		void CalcLock(Game *game);
		void LoopWhenStopped(Game *game);
		void AllowNext(bool preventNext);
		void LockUnlock(Game *game);
};

// ------------------------------------------------------------------------- End

#endif // LOCKS_H
