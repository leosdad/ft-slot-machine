
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
 * Sets the current bet to the amount given, according to the
 * rule `maxbet = int(max(1, (nCoins + 2) / 3))`
 */
uint8_t Game::setBet(int8_t bet)
{
	if(nCoins == 0) {
		currentBet = 0;
	} else {
		currentBet = constrain(min(max(1, (nCoins + 2) / 3), bet), 0, MAXBET);
	}
	
	return currentBet;
}

/**
 * Initializes a new game.
 */
void Game::init(uint16_t initialCoins)
{
	for(int l = 0; l < NPAYLINES; l++) {
		paylines[l].Payoff = 0;
	}

	lastSpinning = -1;
	playing = false;
	spinning = false;
	currentBet = 0;
	nCoins = initialCoins;
	spinPayoff = 0;
	lastFeature = SpecialFeatures::NONE;

	// Serial.println("+++ nCoins " + String(nCoins));

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

	// Print spin data, current bet and coins

	Serial.println();
	Serial.print("---- Spin #");
	Serial.print(totalSpins);
	Serial.print(" ----");
	Serial.println();

	Serial.print("Current bet: ");
	Serial.print(currentBet);
	Serial.print(" / Coins: ");
	Serial.println(nCoins);

	// Serial.print("Extra turns: ");
	// for(int i = 0; i < NREELS; i++) {
	// 	Serial.print(reels[i].extraTurns);
	// 	Serial.print(" ");
	// }
	// Serial.println();

	// Print lines and payoffs

	for(int l = 0; l < NPAYLINES; l++) {
		Serial.print("[ ");
		Serial.print(symbolNames[paylines[l].GetLineSymbol(l, 0, reels[0])]);
		Serial.print(" ");
		Serial.print(symbolNames[paylines[l].GetLineSymbol(l, 1, reels[1])]);
		Serial.print(" ");
		Serial.print(symbolNames[paylines[l].GetLineSymbol(l, 2, reels[2])]);
		Serial.print(" ] ");
		if(paylines[l].Payoff) {
			Serial.println("* " + String(paylines[l].Payoff) + " *");
		} else {
			Serial.println();
		}
	}

	// Print payoff data

	if(spinPayoff > 0) {
		Serial.println("**** Total payoff: " + String(spinPayoff) + " ****");

		if(lastFeature == SpecialFeatures::JACKPOT) {
			Serial.println("#### JACKPOT! ####");
		} else if(lastFeature == SpecialFeatures::BONUS) {
			Serial.println("#### Bonus ####");
		}

		Serial.println("Next coins: " + String(nCoins + spinPayoff));
	} else {
		Serial.println("(No payoff)");
	}

	// Print total wins

	Serial.println("Total wins: " + String(totalWins));
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
		lastFeature = payoffs.GetHighestFeature();
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
 * Increments the bet by the amount given according to the rules in setBet().
 * If bet is not given, just enforces the maximum bet value.
 */
uint8_t Game::ChangeBet(int8_t bet = 0)
{
	currentBet = setBet(currentBet + bet);
	return currentBet;
}

/**
 * Initializes the game. Must be called from the main Setup() function.
 */
void Game::Setup(uint16_t initialCoins)
{
	setupReels();
	init(initialCoins);
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
			nCoins += spinPayoff;
		}
		lastSpinning = spinning;
	}

	return endSpin;
}

// ------------------------------------------------------------------------- End
