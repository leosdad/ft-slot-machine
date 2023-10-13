
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

	spinsLeft = DEFSPINSLEFT;
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
		Serial.println("---- Home spin with " + String(nCoins) + " coins ----");
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

	uint8_t nLocked = 0;
	for(int i = 0; i < NREELS; i++) {
		if(locks.IsLocked(i)) {
			nLocked++;
		}
	}	

	Serial.println();
	Serial.println("---- Spins left: " + String(spinsLeft) + " ----");
	Serial.println("Current bet: " + String(currentBet)
		+ " / Coins: " + String(nCoins)
		+ " / Locked: " + String(nLocked)
	);

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
			Serial.println("* " + String(paylines[l].Payoff));
		} else {
			Serial.println();
		}
	}

	// Print payoff data

	if(doublePay > 0) {
		Serial.println("Double pay for next " + String(doublePay) + " spin(s)");
	}

	if(spinPayoff > 0) {
		Serial.println("**** Total payoff: " + String(spinPayoff * multiplier) + " ****");

		if(lastFeature == SpecialFeatures::TOPSCORE) {
			Serial.println("#### Top score ####");
		} else if(lastFeature == SpecialFeatures::BONUS) {
			Serial.println("#### Bonus ####");
		} else if(lastFeature == SpecialFeatures::DOUBLE) {
			Serial.println("#### Double pay ####");
		}

		Serial.println("Next coins: " + String(nCoins + spinPayoff * multiplier));
	} else {
		Serial.println("(No payoff)");
	}
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

		// Gets highest feature

		lastFeature = payoffs.GetHighestFeature(this);
		if(lastFeature == SpecialFeatures::BONUS) {
			spinsLeft += BONUSSPINS;
		} else if(lastFeature == SpecialFeatures::DOUBLE) {
			doublePay = DOUBLESPINS + 1;
		}

		// Calculates payoff

		if(doublePay == DOUBLESPINS + 1) {
			// Just awarded a double payment, so doesn't pay double right now
			payoffs.SetMultiplier(1);
			payoffs.CalculateTotalPayoff(this);
			doublePay--;
		} else if(doublePay > 0) {
			// Awarded a double pay. If the spin doesn't pay off, try next
			payoffs.SetMultiplier(2);
			payoffs.CalculateTotalPayoff(this);
			doublePay = spinPayoff > 0 ? 0 : doublePay - 1;
		} else {	// doublePay == 0
			payoffs.SetMultiplier(1);
			payoffs.CalculateTotalPayoff(this);
		}

		playing = true;
		spinsLeft--;
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

void Game::SetCoins(uint16_t _nCoins)
{
	nCoins = _nCoins;
}

/**
 * Rotates all reels backwards for a very short time
 */
bool Game::BounceReelsBack()
{
	for(int i = 0; i < NREELS; i++) {
		reels[i].BounceBack();
	}
}

/**
 * Rotates all reels forward for a very short time
 */
bool Game::BounceReelsForward()
{
	for(int i = 0; i < NREELS; i++) {
		reels[i].BounceForward();
	}
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
			nCoins += spinPayoff * multiplier;
		}
		lastSpinning = spinning;
	}

	return endSpin;
}

// ------------------------------------------------------------------------- End
