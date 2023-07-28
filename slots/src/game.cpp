
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
