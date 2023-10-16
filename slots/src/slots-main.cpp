
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include <arduino-timer.h>
#include "ball-feeder.h"
#include "cheering.h"
#include "display.h"
#include "drivers/led-matrix.h"
#include "drivers/pwm-micros.h"
#include "drivers/sound.h"
#include "game.h"
#include "locks.h"
#include "reel.h"
#include "slots-main.h"

// ------------------------------------------------------------ Global variables

// Global objects

BallFeeder feeder;
Cheering cheers;
Display display;
Game game;
LedMatrix ledMatrix;
Locks locks;
Sound sound;

auto cheerTimer = timer_create_default();
auto endGameTimer = timer_create_default();
auto leverAnimTimer = timer_create_default();
auto lifeFlashTimer = timer_create_default();
auto lifeTimer = timer_create_default();
auto updateBetTimer = timer_create_default();

// Global variables

bool displayUpdated = false;
bool firstSpin = true;
bool leverPulled = false;
bool reelsLocked = false;
bool waitingForRestart = false;
bool showingLife = true;
bool showLifeValue = true;
extern SlotsMain slotsMain;
uint16_t startCoins = STARTCOINS;
uint8_t lastBet = 255;
uint8_t lastCoins = -1;
uint8_t leverFrame = 0;
uint8_t nLocked = 0;
uint8_t lastLocked = 255;
uint8_t lockPenalty = 0;
uint8_t lastPenalty = 255;
GameResult gameResult = GameResult::NONE;

// ------------------------------------------------------------ Global functions

void calcLockPenalty()
{
	if(!nLocked) {
		lockPenalty = 0;
	}
	lockPenalty = nLocked * (game.nCoins * game.currentBet / LOCKPENALTYDIV);
}

bool updateDisplay(void *)
{
	showingLife = false;
	display.blink(false);
	displayUpdated = true;

	if(game.nCoins > 0) {
		if(game.currentBet > 0) {
			display.showBet(game.currentBet);
			display.showNumber(game.nCoins - lockPenalty, true);
		} else {
			display.showCentered("No bet");
		}
	}
	return true;
}

bool toggleLifeIndicator(void *)
{
	if(showingLife) {
		showLifeValue = !showLifeValue;
 		display.showLife(showLifeValue ? game.spinsLeft : 0);
	}
}

/**
 * Displays the animated lever.
 */
bool showAnimLever(void *)
{
	char ch;

	if(leverFrame < 10) {
		ch = '\x1a' + (leverFrame < 5 ? leverFrame : 9 - leverFrame);
		leverAnimTimer.in(LEVERANIMRATE, showAnimLever);
		leverFrame++;
	} else if(leverFrame == 11) {
		ch = ' ';
		leverAnimTimer.in(LEVERWAIT, showAnimLever);
		leverFrame++;
	} else {
		leverAnimTimer.in(LEVERANIMDELAY, showAnimLever);
		updateDisplay(NULL);
		leverFrame = 0;
	}
	ledMatrix.printChar(ch, 0);
}

/**
 * Called periodically.
 */
bool updateBet(void *)
{
	if(game.currentBet != lastBet) {
		if(leverFrame != 0) {
			ledMatrix.printText(" ", MX_TEXTPOS);
		}
		leverAnimTimer.cancel();
		calcLockPenalty();
		updateDisplay(NULL);
		leverAnimTimer.in(LEVERANIMDELAY, showAnimLever);
		lastBet = game.currentBet;
	}

	if(nLocked != lastLocked) {
		calcLockPenalty();
		updateDisplay(NULL);
		lastLocked = nLocked;
	}
	
	if(lockPenalty != lastPenalty) {
		lastPenalty = lockPenalty;
	}

	return true;
}

bool showRemainingSpins(void *)
{
	if(game.spinsLeft <= SHOWREMAINING) {
		if(game.spinsLeft <= REMAINWARNING) {
			lifeFlashTimer.every(LIFETOGGLE, toggleLifeIndicator);
			showingLife = true;
		} else {
			lifeFlashTimer.cancel();
			showingLife = false;
			showLifeValue = true;
		}
		display.showLife(game.spinsLeft);
	}
	lifeTimer.in(REMAINRESET, updateDisplay);
	lifeTimer.in(REMAINREPEAT, showRemainingSpins);
}

bool restartDisplay(void *)
{
	// display.clearAll();
	updateBetTimer.every(UPDATEBET, updateBet);
	leverAnimTimer.in(LEVERANIMDELAY, showAnimLever);
	lifeTimer.in(REMAINSTART, showRemainingSpins);
	updateDisplay(NULL);
}

/**
 * Callback for the LedMatrix::wrapText function. Yewlds to the main loop.
 */
bool wrapLoop()
{
	unsigned long timeout = millis() + WRAPDELAY;
	while(millis() < timeout) {
		cheerTimer.tick();
		cheers.Loop(true);
		// Quick hack
		if(!digitalRead(leverButtonPin)) {
			return true;
		}
	}
	return false;
}

bool displayEndGameMessage(void *)
{
	char str[80];
	const char *pullLever = "Pull lever to restart ...";

	display.clearAll();

	switch(gameResult) {
		case GameResult::VICTORY:
			sprintf(str, " You won with %d points! %s", game.nCoins, pullLever);
			break;
		case GameResult::NO_COINS_LEFT:
			sprintf(str, " No more coins. %s", pullLever);
			break;
		case GameResult::NO_SPINS_LEFT:
			sprintf(str, " No spins left. %s", pullLever);
			break;
	}

	ledMatrix.wrapText(str, wrapLoop);
}

void displayAward()
{
	static const char* feats[] = {"", "Double", "Bonus", "Top"};

	displayUpdated = false;
	cheerTimer.cancel();
	updateBetTimer.cancel();
	lifeFlashTimer.cancel();
	leverAnimTimer.cancel();
	lifeTimer.cancel();

	display.showCentered(feats[(uint16_t)game.lastAward]);

	cheerTimer.in(CHEERENDTIME, restartDisplay);
}

void cheerIfNeeded()
{
	CheerLevel cheerLevel;

	switch(gameResult) {

		case GameResult::VICTORY:
			cheerLevel = CheerLevel::VICTORY;
			sound.Play((uint8_t)Sounds::GAME_WON);
			break;

		case GameResult::NO_COINS_LEFT:
		case GameResult::NO_SPINS_LEFT:
			cheerLevel = CheerLevel::NONE;
			sound.Play((uint8_t)Sounds::GAME_WON);
			break;

		default:	// GameResult::NONE
			if(game.lastAward > Awards::NONE) {

				displayAward();

				cheerLevel = CheerLevel::BIG_WIN;
				if(game.lastAward == Awards::TOPSCORE) {
					display.blink(true, TOPSCOREBLINK);
					sound.Play((uint8_t)Sounds::CHEER_TOPSCORE);
				} else {
					sound.Play((uint8_t)Sounds::CHEER_BONUS);
				}

			} else if(game.nCoins > lastCoins) {
				cheerLevel = CheerLevel::WIN;
				sound.Play((uint8_t)Sounds::CHEER_WIN);
			} else if(game.nCoins == lastCoins) {
				cheerLevel = CheerLevel::DRAW;
				sound.Play((uint8_t)Sounds::CHEER_DRAW);
			} else {	// game.nCoins < lastCoins
				cheerLevel = CheerLevel::NONE;
				sound.Stop();
			}
			break;
	}

	cheers.Start(cheerLevel);
}

/**
 * Called when the game ends.
 */
void endGame()
{
	lifeFlashTimer.cancel();
	lifeTimer.cancel();
	leverAnimTimer.cancel();

	waitingForRestart = true;
	game.playing = false;

	if(gameResult == GameResult::VICTORY) {
		#if !SIMULATE
		feeder.Feed();
		#endif
		startCoins = min(MAXSTARTCOINS, STARTCOINS + (game.nCoins - VICTORYPOINTS) / 2);
		sound.Play((uint8_t)Sounds::GAME_WON);
		#if DEBUGINFO
		Serial.println("\n--- VICTORY ---\n");
		#endif
	} else {
		startCoins = STARTCOINS;
		sound.Play((uint8_t)Sounds::GAME_LOST);
		#if DEBUGINFO
		if(gameResult == GameResult::NO_COINS_LEFT) {
			Serial.println("\n--- NO MORE COINS ---\n");
		} else {
			Serial.println("\n--- NO SPINS LEFT ---\n");
		}
		#endif
	}

	endGameTimer.in(1000, displayEndGameMessage);
}

/**
 * Called once each time a spin ends.
 */
void endSpin()
{
	if(!firstSpin) {

		if(game.nCoins > 0) {

			lastLocked = 255;
			lastPenalty = 255;
			showLifeValue = true;

			updateBetTimer.every(UPDATEBET, updateBet);
			lifeTimer.in(REMAINSTART, showRemainingSpins);

			// Enforces the maximum bet value if needed
			game.ChangeBet();

			// Game won? Game lost? Needs cheering?

			gameResult = GameResult::NONE;
			if((game.nCoins >= VICTORYPOINTS) && (game.spinsLeft >= 0)) {
				gameResult = GameResult::VICTORY;
			} else if(game.spinsLeft <= 0) {
				gameResult = GameResult::NO_SPINS_LEFT;
			}

			cheerIfNeeded();

			if(gameResult > GameResult::NONE) {
				endGame();
			}

		} else {
			gameResult = GameResult::NO_COINS_LEFT;
			endGame();
		}
	}

	leverPulled = false;
	leverAnimTimer.in(LEVERANIMDELAY, showAnimLever);
}

/**
 * Called when the lever is pulled.
 */
void bounceReels()
{
	updateBetTimer.cancel();
	leverAnimTimer.cancel();
	lifeFlashTimer.cancel();
	lifeTimer.cancel();

	if(!waitingForRestart && !leverPulled &&
		game.spinsLeft <= SHOWREMAINING) {
		updateDisplay(NULL);
	}

	game.BounceReelsBack();
	leverPulled = true;
}

/**
 * Called when a new spin is about to start.
 */
void spin()
{
	cheers.Stop();

	if(game.nCoins > 0 && game.currentBet > 0) {

		game.SetCoins(game.nCoins - lockPenalty);
		lastCoins = game.nCoins;
		lockPenalty = 0;
		nLocked = 0;

		reelsLocked = false;
		for(int i = 0; i < NREELS; i++) {
			if(locks.IsLocked(i)) {
				reelsLocked = true;
			}
		}

		if(game.spinsLeft <= REMAINWARNING) {
			sound.Play((uint8_t)Sounds::END_IS_NEAR);
		}

		game.StartSpin(false);
	}
}

// ---------------------------------------------------- Private member functions

void SlotsMain::ioSetup()
{
	// Set output pin modes

	pinMode(servoPin, OUTPUT);
	pinMode(signalLED1Gnd, OUTPUT);
	pinMode(signalLED2Gnd, OUTPUT);

	// Set fixed levels

	digitalWrite(signalLED1Gnd, LOW);
	digitalWrite(signalLED2Gnd, LOW);

	// Initialize ezButtons

	startLever = ezButton(leverButtonPin);
	increaseBet = ezButton(increaseBetPin);
	decreaseBet = ezButton(decreaseBetPin);

	startLever.setDebounceTime(EZBTNDEBOUNCE);
	increaseBet.setDebounceTime(EZBTNDEBOUNCE);
	decreaseBet.setDebounceTime(EZBTNDEBOUNCE);
}

void SlotsMain::inputLoop()
{
	// process ezButtons

	increaseBet.loop();
	decreaseBet.loop();
	startLever.loop();

	// Read ezButtons values

	if(waitingForRestart) {
		if(startLever.isPressed()) {
			bounceReels();
		} else if(slotsMain.startLever.isReleased()) {
			display.clearAll();
			slotsMain.Restart();
			waitingForRestart = false;
		}
	} else {
		if(!game.spinning) {

			if(startLever.isPressed()) {
				if(game.currentBet) {
					bounceReels();
				} else {
					game.BounceReelsBack();
				}
			} else if(startLever.isReleased()) {
				if(game.currentBet) {
					firstSpin = false;
					spin();
				} else {
					game.BounceReelsForward();
				}
			}
			
			if(!leverPulled) {
				if(increaseBet.isPressed()) {
					game.ChangeBet(1);
					if(updateBetTimer.empty()) {
						updateBetTimer.every(UPDATEBET, updateBet);
					}
				} else if(decreaseBet.isPressed()) {
					game.ChangeBet(-1);
					if(updateBetTimer.empty()) {
						updateBetTimer.every(UPDATEBET, updateBet);
					}
				}
			}
		}
	}
}

// ----------------------------------------------------- Public member functions

void SlotsMain::Setup()
{
	Serial.begin(BAUDRATE);
	Serial.println("Slots game started.");
	Serial.println();

	// Initialize pins

	ioSetup();

	// Sets up objects

	sound.Setup(VOLUME);
	#if !SIMULATE
	feeder.Setup(servoPin);
	feeder.Return();
	#endif
	ledMatrix.setup();
	display.setup();
	display.scroll(" Wait ");
	cheers.Setup();
	updateBetTimer.every(UPDATEBET, updateBet);
	locks.Setup();
	game.Setup(startCoins);
	sound.Play((uint8_t)Sounds::HELLO);

	// Perform a first (home) spin

	game.StartSpin(true);
}

void SlotsMain::Restart()
{
	// Resets variable values

	lastBet = 255;
	lastCoins = -1;
	firstSpin = true;
	reelsLocked = false;
	leverFrame = 0;
	displayUpdated = false;
	waitingForRestart = false;
	gameResult = GameResult::NONE;

	Serial.println();
	Serial.println("Slots game restarted.");
	Serial.println();

	// Initialize pins

	ioSetup();

	// Sets up objects

	#if !SIMULATE
	feeder.Return();
	#endif
	display.scroll(" Wait");
	updateBetTimer.every(UPDATEBET, updateBet);
	locks.Reset();
	game.Setup(startCoins);
	sound.Play((uint8_t)Sounds::HELLO);

	// Perform a home spin

	game.StartSpin(true);
}

void SlotsMain::Loop()
{
	if(!game.spinning) {
		cheerTimer.tick();
		endGameTimer.tick();
		leverAnimTimer.tick();
		lifeFlashTimer.tick();
		lifeTimer.tick();
		updateBetTimer.tick();

		inputLoop();
		feeder.Loop();
		display.loop();
	}

	if(game.Loop()) {
		endSpin();
	}
	nLocked = locks.Loop(
		!(game.spinning || leverPulled),
		!(firstSpin || game.spinPayoff || reelsLocked),
		game.currentBet
	);
	cheers.Loop(!game.spinning && game.spinPayoff);
}

#pragma endregion

// ------------------------------------------------------------------------- End
