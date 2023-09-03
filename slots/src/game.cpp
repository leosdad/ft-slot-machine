
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "game.h"
#include "payoffs.h"
#include "locks.h"

// ------------------------------------------------------------ Global variables

Payoffs payoffs;
Reel reels[NREELS];
extern Locks locks;

// ---------------------------------------------------- Private member functions

/**
 * Set up the hardware pins and some variables.
 */
void Game::setupReels()
{
	for(int i = 0; i < NREELS; i++) {
		reels[i].Setup(i);
	}
}

/**
 * Sets the current bet to the amount given.
 */
uint8_t Game::setBet(int8_t bet)
{
	currentBet = constrain(min(nCoins, bet), 0, MAXBET);
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
 * @param home Spin to the home position.
 * @returns `false` if no reels are allowed to spin.
 */
bool Game::StartSpin(bool home)
{
	if(currentBet == 0) {
		return false;
	}

	newBall = false;
	uint8_t xtraTurns = 0;

	// Starts each reel

	for(int i = 0; i < NREELS; i++) {
		if(!locks.IsLocked(i)) {
			xtraTurns = reels[i].Start(home, xtraTurns);
		}
	}

	// At this point the game already knows the results of this spin
	
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

	return true;
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
	setupReels();
	init();
}

/**
 * Returns `true` once when spinning has ended.
 */
bool Game::Loop()
{
	bool endSpin = false;

	spinning = false;

	for(int i = 0; i < NREELS; i++) {
		if(reels[i].Loop()) {
			spinning = true;
		}
	}

	if(spinning != lastSpinning) {
		if(!spinning) {
			endSpin = true;
		}
		lastSpinning = spinning;
	}

	return endSpin;
}

// ------------------------------------------------------------------------- End
