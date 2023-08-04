
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

	public:

		int blinkLedState;

		Locks();

		void Loop(Game *game);
		void AllowNext(Game *game, bool prevent = false);
		void LockUnlock(Game *game);
		void CalcLock(Game *game);
};

// ------------------------------------------------------------------------- End

#endif // LOCKS_H
