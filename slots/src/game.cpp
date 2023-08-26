
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "game.h"
#include "lock.h"
#include "payoffs.h"

// ------------------------------------------------------------ Global variables

Payoffs payoffs;
Reel reels[NREELS];
extern Lock lock[NREELS];

// ---------------------------------------------------- Private member functions

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
			i,
			motorPins[i],
			encoderPins[i],
			homeSensorPins[i],
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

/**
 * Print debug data on the serial port for each spin.
 */
void Game::printDebugData(bool home)
{
	if(home) {
		Serial.println("---- Home spin ----");
		Serial.println();
		return;
	}

	static const char *symbolNames[NSYMBOLS + 1] = {
		"Any",
		"Seven",
		"Banana",
		"Cherry",
		"Watermelon",
		"Bell",
		"Orange",
		"Lemon",
		"Grapes"
	};

	Serial.println();
	Serial.print("---- Spin #");
	Serial.print(totalSpins);
	Serial.print(" ----");
	Serial.println();

	Serial.print("Current bet: ");
	Serial.print(currentBet);
	Serial.print(" / Coins: ");
	Serial.println(nCoins);

	Serial.print("Extra turns: ");
	for(int i = 0; i < NREELS; i++) {
		Serial.print(reels[i].extraTurns);
		Serial.print(" ");
	}
	Serial.println();

	// Serial.print("Balls: ");
	// Serial.println(nBalls);
	// Serial.print(" ");
	// Serial.println(newBall);

	Serial.println("Lines and payoffs:");
	for(int l = 0; l < NPAYLINES; l++) {
		Serial.print("** ");
		Serial.print(symbolNames[paylines[l].GetLineSymbol(l, 0, reels[0])]);
		Serial.print(" ");
		Serial.print(symbolNames[paylines[l].GetLineSymbol(l, 1, reels[1])]);
		Serial.print(" ");
		Serial.print(symbolNames[paylines[l].GetLineSymbol(l, 2, reels[2])]);
		Serial.print(" ** ");
		Serial.println(paylines[l].Payoff);
	}

	Serial.print("Total payoff: ");
	Serial.println(spinPayoff);

	Serial.print("Total wins: ");
	Serial.println(totalWins);
}

// ----------------------------------------------------- Public member functions

/**
 * Starts a new spin.
 */
void Game::StartSpin(bool home)
{
	newBall = false;
	uint8_t xtraTurns = 0;

	// Starts each reel

	for(int i = 0; i < NREELS; i++) {
		if(!lock[i].IsLocked()) {
			xtraTurns = reels[i].Start(home, xtraTurns);
		}
	}
	
	if(!home) {
		payoffs.CalculateTotalPayoff(this);
		playing = true;
		totalSpins++;
	}

	if(playing) {
		nCoins = constrain(nCoins - currentBet, 0, MAXCOINS);
	}

	#if DEBUGINFO
		printDebugData(home);
	#endif
}

/**
 * Increments the bet by the amount given.
 */
uint8_t Game::ChangeBet(int8_t bet)
{
	currentBet = constrain(min(nCoins, currentBet + bet), 0, MAXBET);
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
