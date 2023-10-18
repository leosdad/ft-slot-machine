
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "game.h"
#include "payouts.h"
#include "locks.h"

// ------------------------------------------------------------ Global variables

Payouts payouts;
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
		paylines[l].Payout = 0;
	}

	lastSpinning = -1;
	multiplier = 1;
	playing = false;
	spinning = false;
	doublePay = 0;
	spinsLeft = DEFSPINSLEFT;
	currentBet = 0;
	nCoins = initialCoins;
	spinPayout = 0;
	lastAward = Awards::NONE;

	// Serial.println("+++ nCoins " + String(nCoins));

	setBet(INITIALBET);
}

/**
 * Print debug data on the serial port for each spin.
 */
void Game::printDebugData(bool home)
{
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

	if(home) {
		Serial.println("---- Home spin with " + String(nCoins) + " coins ----");
	}

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

	// Print lines and payouts

	for(int l = 0; l < NPAYLINES; l++) {
		Serial.print("[ ");
		Serial.print(symbolNames[paylines[l].GetLineSymbol(l, 0, reels[0])]);
		Serial.print(" ");
		Serial.print(symbolNames[paylines[l].GetLineSymbol(l, 1, reels[1])]);
		Serial.print(" ");
		Serial.print(symbolNames[paylines[l].GetLineSymbol(l, 2, reels[2])]);
		Serial.print(" ] ");
		if(paylines[l].Payout) {
			Serial.println("* " + String(paylines[l].Payout));
		} else {
			Serial.println();
		}
	}

	// Print payout data

	if(doublePay > 0) {
		Serial.println("Double pay for next " + String(doublePay) + " spin(s)");
	}

	if(spinPayout > 0) {
		Serial.println("**** Total payout: " + String(spinPayout * multiplier) + " ****");

		if(lastAward == Awards::TOPSCORE) {
			Serial.println("#### Top score ####");
		} else if(lastAward == Awards::BONUS) {
			Serial.println("#### Bonus ####");
		} else if(lastAward == Awards::DOUBLE) {
			Serial.println("#### Double pay ####");
		}

		Serial.println("Next coins: " + String(nCoins + spinPayout * multiplier));
	} else {
		Serial.println("(No payout)");
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

		// Gets highest award

		lastAward = payouts.GetHighestAward(this);

		if(lastAward == Awards::BONUS) {
			spinsLeft += BONUSSPINS;
		} else if(lastAward == Awards::DOUBLE) {
			doublePay = DOUBLESPINS + 1;
		}

		// Calculates payout

		if(doublePay == DOUBLESPINS + 1) {
			// Just awarded a double payment, so doesn't pay double right now
			payouts.SetMultiplier(1);
			payouts.CalculateTotalPayout(this);
			doublePay--;
		} else if(doublePay > 0) {
			// Awarded a double pay. If the spin doesn't pay off, try next
			payouts.SetMultiplier(2);
			payouts.CalculateTotalPayout(this);
			doublePay = spinPayout > 0 ? 0 : doublePay - 1;
		} else {	// doublePay == 0
			payouts.SetMultiplier(1);
			payouts.CalculateTotalPayout(this);
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
	#if !SIMULATE
	for(int i = 0; i < NREELS; i++) {
		reels[i].BounceBack();
	}
	#endif
}

/**
 * Rotates all reels forward for a very short time
 */
bool Game::BounceReelsForward()
{
	#if !SIMULATE
	for(int i = 0; i < NREELS; i++) {
		reels[i].BounceForward();
	}
	#endif
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
			nCoins += spinPayout * multiplier;
		}
		lastSpinning = spinning;
	}

	return endSpin;
}

// ------------------------------------------------------------------------- End
