
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

		unsigned long previousMs;
		bool allowNext;
		int16_t prevBrightness;
		int16_t fadeAmount;

	public:

		// Enums

		enum class NextState {
			AUTO = 0,
			ALLOWED,
			FORBIDDEN
		};

		int16_t brightness;
		bool changeBrightness;

		Locks();

		void Loop(Game *game);
		void AllowNext(NextState state);
		void LockUnlock(Game *game);
		void CalcLock(Game *game);
};

// ------------------------------------------------------------------------- End

#endif // LOCKS_H
