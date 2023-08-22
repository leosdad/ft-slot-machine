
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "game.h"
// #include "locks.h"
#include "payoffs.h"

// Locks locks;
Payoffs payoffs;
Reel reels[NREELS];

// ---------------------------------------------------- Private member functions

// void Game::loopWhenStopped()
// {
// 	// locks.LoopWhenStopped(this);

// 	bool changed = false;

// 	for(int i = 0; i < NREELS; i++) {
// 		if(reels[i].LoopWhenStopped()) {
// 			changed = true;
// 		}
// 	}

// 	if(changed) {
// 		locks.CalcLocked(this);
// 	}
// }

/**
 * Set up the hardware pins and some variables.
 */
void Game::setupReels(
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

/**
 * Sets the current bet to the amount given.
 */
uint8_t Game::setBet(int8_t bet)
{
	currentBet = constrain(min(nCoins, bet), 0, MAXBET);
	// locks.CalcLocked(this);
	return currentBet;
}

/**
 * Actions after a spin is completed.
 */
void Game::stopSpin()
{
	// spinning = false;
	// locks.LockUnlock(this);

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
}

/**
 * Initializes a new game.
 */
void Game::init()
{
	for(int l = 0; l < NPAYLINES; l++) {
		paylines[l].Payoff = 0;
	}

	nCoins = STARTCOINS;
	setBet(INITIALBET);
}

// ----------------------------------------------------- Public member functions

/**
 * Starts a new spin.
 */
void Game::StartSpin(bool home)
{
	newBall = false;
	// locks.AllowNext(home);

	uint8_t xtraTurns = 0;

	// Starts each reel

	for(int i = 0; i < NREELS; i++) {
		xtraTurns = reels[i].Start(home, xtraTurns);
	}
	
	if(!home) {
		payoffs.CalculateTotalPayoff(this);
		playing = true;
	}

	totalSpins++;

	if(playing) {
		nCoins = constrain(nCoins - currentBet, 0, MAXCOINS);
	}
}

/**
 * Increments the bet by the amount given.
 */
uint8_t Game::ChangeBet(int8_t bet)
{
	currentBet = constrain(min(nCoins, currentBet + bet), 0, MAXBET);
	// locks.CalcLocked(this);
	return currentBet;
}

/**
 * Initializes the game. Must be called from the main Setup() function.
 */
void Game::Setup()
{
	setupReels(motorOutPins, encoderPins, homeSensorPins,
	 	lockButtonPins, lockLEDPins, motorSpeed);
	init();
}

/**
 * Returns `true` if the reels are still spinning.
 */
bool Game::Loop()
{
	bool isSpinning = false;

	for(int i = 0; i < NREELS; i++) {
		if(reels[i].Loop()) {
			isSpinning = true;
		}
	}
	return isSpinning;
}

// ------------------------------------------------------------------------- End
