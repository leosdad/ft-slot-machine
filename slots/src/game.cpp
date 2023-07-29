
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "game.h"
#include "locks.h"

Locks locks;

// ----------------------------------------------------- Public member functions

/**
 * Initializes the game. Must be called from the main Setup() function.
 */
void Game::Setup(Reel _reels[NREELS])
{
	for(int i = 0; i < NREELS; i++) {
		reels[i] = _reels[i];
	}

	for(int l = 0; l < NPAYLINES; l++) {
		paylines[l].Payoff = 0;
	}
}

void Game::LoopWhenSpinning()
{
	for(int i = 0; i < NREELS; i++) {
		reels[i].LoopWhenSpinning();
	}
}

void Game::LoopWhenStopped()
{
	locks.Loop(this);
	for(int i = 0; i < NREELS; i++) {
		reels[i].LoopWhenStopped(locks.blinkLedState);
	}
}

/**
 * Sets the current bet to the amount given.
 */
uint8_t Game::SetBet(int8_t bet)
{
	currentBet = constrain(min(nCoins, bet), 0, MAXBET);
	return currentBet;
}

/**
 * Increments the bet by the amount given.
 */
uint8_t Game::ChangeBet(int8_t bet)
{
	currentBet = constrain(min(nCoins, currentBet + bet), 0, MAXBET);
	return currentBet;
}

void Game::StartReels(bool home)
{
	locks.AllowNext(home || nCoins == 0 ? Locks::NextState::FORBIDDEN :
		Locks::NextState::AUTO);

	uint8_t additionalTurns = 0;

	for(int i = 0; i < NREELS; i++) {
		additionalTurns = reels[i].Start(home, additionalTurns);
	}
}

/**
 * Actions after a spin is completed.
 */
void Game::StopSpin()
{
	spinning = false;
	locks.LockUnlock(this);
	if(playing) {
		nCoins = constrain(nCoins + spinPayoff * currentBet, 0, MAXCOINS);
	}
	if(spinPayoff) {
		locks.AllowNext(Locks::NextState::FORBIDDEN);
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

/**
 * Set up the hardware pins and some variables.
 */
void Game::InitReels(
	const uint8_t motorPins[NREELS][2],
	const uint8_t encoderPin[NREELS],
	const uint8_t homeSensorPin[NREELS],
	const uint8_t lockButtonPin[NREELS],
	const uint8_t lockLEDPin[NREELS],
	const uint8_t motorSpeed[NREELS]
) {
	for(int i = 0; i < NREELS; i++) {
		reels[i].Setup(
			motorPins[i],
			encoderPin[i],
			homeSensorPin[i],
			lockButtonPin[i],
			lockLEDPin[i],
			motorSpeed[i]
		);
	}
}

// ------------------------------------------------------------------------- End
