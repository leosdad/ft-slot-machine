
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "game.h"

// --------------------------------------------------------------- Class members

void Game::Setup(Reel myReels[NREELS])
{
	for(int i = 0; i < NREELS; i++) {
		reels[i] = myReels[i];
	}
	// reels = myReels;

	for(int l = 0; l < NPAYLINES; l++) {
		paylines[l].Payoff = 0;
	}
}

/**
 * Sets the bet to the amount given.
 */
uint8_t Game::SetBet(uint8_t bet)
{
	currentBet = constrain(bet, 0, MAXBET);
	return currentBet;
}

/**
 * Increments the bet by the amount given.
 */
uint8_t Game::ChangeBet(int8_t bet)
{
	currentBet = constrain(currentBet + bet, 0, MAXBET);
	return currentBet;
}

void Game::ProcessWhenSpinning()
{
	for(int i = 0; i < NREELS; i++) {
		reels[i].ProcessWhenSpinning();
	}
}

void Game::ProcessWhenStopped(int blinkLedState)
{
	for(int i = 0; i < NREELS; i++) {
		CalcLock();
		reels[i].ProcessWhenStopped(blinkLedState);
	}
}

void Game::StartReels(bool home)
{
	for(int i = 0; i < NREELS; i++) {
		reels[i].Start(home, 0);
	}
}

void Game::ResetReels(bool start)
{
	for(int i = 0; i < NREELS; i++) {
		reels[i].Reset(start);
	}
}

/*
Besides, reel locking will not be possible in the following situations:

In the first turn of each game session 
After a turn where you have locked one or two reels 
After a turn where you have won in one or more paylines 
After a turn where you have lost all your money 
After a turn where you have configured a winning combination, even if you have not bet on it. For example, if you have bet 1 coin for the last turn and obtained two watermelons in line 2 (upper payline), you will not be able to lock any reel. 

*/

/**
 * Lock / unlock logic.
 */
void Game::CalcLock()
{
	// TODO: this logic does not need to be inside the loop; only
	// after a lock button or the bet incr/decr are pressed

	uint8_t currentLocked = 0;
	uint8_t maxLockable = min(2, currentBet / 3);

	for(int i = 0; i < NREELS; i++) {
		if(reels[i].locked) {
			currentLocked++;
		}
	}

	if(currentLocked < maxLockable) {
		for(int i = 0; i < NREELS; i++) {
			reels[i].lockable = true;
		}
	} else if(currentLocked == maxLockable) {
		for(int i = 0; i < NREELS; i++) {
			if(!reels[i].locked) {
				reels[i].lockable = false;
				reels[i].locked = false;
			}
		}
	} else {	// (currentLocked > maxLockable)
		for(int i = NREELS - 1; i >= 0;  i--) {
			if(reels[i].locked) {
				reels[i].locked = false;
				currentLocked--;
				if(currentLocked <= maxLockable) {
					break;
				}
			}
		}
	}
}

/**
 * Lock / unlock logic after a spin is completed.
 */
void Game::LockUnlock()
{
	bool allowNext = true;

	for(int i = 0; i < NREELS; i++) {
		if(reels[i].locked) {
			allowNext = false;
			break;
		}
	}

	if(allowNext) {
		for(int i = 0; i < NREELS; i++) {
			reels[i].lockable = true;
			reels[i].locked = false;
		}
	} else {
		for(int i = 0; i < NREELS; i++) {
			reels[i].lockable = false;
			reels[i].locked = false;
		}
	}
}

/**
 * Returns true if all reels are in idle state.
 */
bool Game::IsIdle()
{
	for(int i = 0; i < NREELS; i++) {
		if(!reels[i].IsIdle()) {
			return false;
		}
	}
	return true;
}

void Game::InitReels(
	const uint8_t motorPins[NREELS][2],
	const uint8_t encoderPin[NREELS],
	const uint8_t homeSensorPin[NREELS],
	const uint8_t lockButtonPin[NREELS],
	const uint8_t lockLEDPin[NREELS],
	const uint8_t motorSpeed[NREELS],
	int **reelComposition
) {
	for(int i = 0; i < NREELS; i++) {
		reels[i].Setup(
			motorPins[i],
			encoderPin[i],
			homeSensorPin[i],
			lockButtonPin[i],
			lockLEDPin[i],
			motorSpeed[i],
			(int *)reelComposition[i]
		);
	}
}

// ------------------------------------------------------------------------- End
