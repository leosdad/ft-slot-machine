
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "game.h"
#include "locks.h"
#include "ball-feeder.h"
#include "cheering.h"

Locks locks;
BallFeeder ballFeeder;
Cheering cheering;

// ----------------------------------------------------- Public member functions

/**
 * Initializes the game. Must be called from the main Setup() function.
 */
void Game::Setup(Reel _reels[NREELS])
{
	ballFeeder.Attach(servoPin);

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
	locks.LoopWhenStopped(this);
	cheering.Loop(this);
	ballFeeder.Loop();

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
	newBall = false;
	cheering.Stop();
	locks.AllowNext(this, home);

	uint8_t xtraTurns = 0;

	for(int i = 0; i < NREELS; i++) {
		xtraTurns = reels[i].Start(home, xtraTurns);
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
		nCoins = constrain(nCoins + spinPayoff, 0, MAXCOINS);

		if(nCoins - nBalls * BALLVALUE > BALLVALUE) {

			nBalls++;
			newBall = true;

			// Feed a new ball
			
			// ballFeeder.Feed();
			// delay(1000);	// TODO
			// ballFeeder.Return();
			// delay(1000);
		}
	}
	if(spinPayoff) {
		locks.AllowNext(this);
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
	const uint8_t encoderPins[NREELS],
	const uint8_t homeSensorPins[NREELS],
	const uint8_t lockButtonPins[NREELS],
	const uint8_t lockLEDPins[NREELS],
	const uint8_t motorSpeed[NREELS]
) {
	for(int i = 0; i < NREELS; i++) {
		reels[i].Setup(
			motorPins[i],
			encoderPins[i],
			homeSensorPins[i],
			lockButtonPins[i],
			lockLEDPins[i],
			motorSpeed[i]
		);
	}
}

// ------------------------------------------------------------------------- End
