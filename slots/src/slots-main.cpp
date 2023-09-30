
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include <arduino-timer.h>
#include "cheering.h"
#include "display.h"
#include "drivers/ball-feeder.h"
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

auto displayTimer = timer_create_default();
auto leverTimer = timer_create_default();
auto updateTimer = timer_create_default();
auto winTimer = timer_create_default();

// Global variables

bool displayUpdated = false;
bool firstSpin = true;
bool leverPulled = false;
bool reelsLocked = false;
bool showWinSymbol = true;
bool waitingForRestart = false;
extern SlotsMain slotsMain;
uint16_t startCoins = STARTCOINS;
uint8_t lastBet = 255;
uint8_t lastCoins = -1;
uint8_t leverFrame = 0;
uint8_t nLocked = 0;
uint8_t lastLocked = 255;
uint8_t lockPenalty = 0;
uint8_t lastPenalty = 255;

// ------------------------------------------------------------ Global functions

/**
 * Displays the flashing "win" symbol.
 */
bool toggleWinSymbol(void *)
{
	if(displayUpdated && game.currentBet > 0) {
		if(!game.spinning && game.spinPayoff) {
			if(showWinSymbol) {
				ledMatrix.printChar('\x1f', COL_SIZE);	// Win symbol
			} else {
				ledMatrix.clearColumns(COL_SIZE, 10);	// Width of Win symbol
			}
		} else {
			ledMatrix.clearColumns(COL_SIZE, 10);	// Width of Win symbol
		}
	}
	showWinSymbol = !showWinSymbol;
}

void calcLockPenalty()
{
	if(!nLocked) {
		lockPenalty = 0;
	}
	
	lockPenalty = nLocked * (game.nCoins * game.currentBet / LOCKPENALTYDIV);
}

bool updateDisplay(void *)
{
	display.blink(false);
	displayUpdated = true;

	if(game.nCoins > 0) {
		if(game.currentBet > 0) {
			display.showBet(game.currentBet);
			display.show(game.nCoins - lockPenalty, true);
		} else {
			display.showCentered("No bet");
		}
	}
	return true;
}

/**
 * Displays the animated lever.
 */
bool showAnimLever(void *)
{
	char ch;

	if(leverFrame < 10) {
		ch = '\x1a' + (leverFrame < 5 ? leverFrame : 9 - leverFrame);
		leverTimer.in(LEVERANIMRATE, showAnimLever);
		leverFrame++;
	} else if(leverFrame == 11) {
		ch = ' ';
		leverTimer.in(LEVERWAIT, showAnimLever);
		leverFrame++;
	} else {
		leverTimer.in(LEVERANIMDELAY, showAnimLever);
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
		leverTimer.cancel();
		calcLockPenalty();
		updateDisplay(NULL);
		leverTimer.in(LEVERANIMDELAY, showAnimLever);
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

bool wrapLoop()
{
	unsigned long timeout = millis() + WRAPDELAY;
	while(millis() < timeout) {
		slotsMain.Loop();
	}
	return !waitingForRestart;
}

/**
 * Called once each time a spin ends.
 */
void endSpin()
{
	static const char* feats[] = {"", "Double", "Bonus", "Jckpot!"};

	if(!firstSpin) {

		if(game.nCoins == 0) {
game_lost:
			winTimer.cancel();
			waitingForRestart = true;
			game.playing = false;
			startCoins = STARTCOINS;
			sound.Play((uint8_t)Sounds::GAME_LOST);
			display.clearAll();
			ledMatrix.wrapText(" Game over. Pull lever to restart ...", wrapLoop);

		} else {

			lastLocked = 255;
			lastPenalty = 255;

			updateTimer.every(UPDATEBET, updateBet);

			// Enforces the maximum bet value if needed
			game.ChangeBet();

			if(game.spinPayoff) {
				showWinSymbol = true;
				winTimer.every(WINTOGGLE, toggleWinSymbol);
			} else {
				winTimer.cancel();
			}

			updateDisplay(NULL);

			// Game won?

			bool victory = false;
			CheerLevel cheerLevel;

			if((game.nCoins >= VICTORYVALUE) && (game.totalSpins <= MAXSPINSTOWIN)) {
				winTimer.cancel();
				feeder.Feed();
				waitingForRestart = true;
				game.playing = false;
				startCoins = min(MAXSTARTCOINS, STARTCOINS + (game.nCoins - VICTORYVALUE) / 2);
				victory = true;

				char str[60];
				sprintf(str, " Game won with %d points! Pull lever to restart ...", game.nCoins);
				display.clearAll();
				ledMatrix.wrapText(str, wrapLoop);
			} else if(game.totalSpins >= MAXSPINSTOWIN) {
				goto game_lost;
			}

			if(victory) {
				cheerLevel = CheerLevel::VICTORY;
				sound.Play((uint8_t)Sounds::GAME_WON);
			} else if(game.lastFeature > SpecialFeatures::NONE) {
				displayUpdated = false;
				cheerLevel = CheerLevel::BIG_WIN;
				display.showCentered(feats[(uint16_t)game.lastFeature]);
				displayTimer.in(DISPLAYTIME, updateDisplay);
				if(game.lastFeature == SpecialFeatures::JACKPOT) {
					display.blink(true, JACKPOTBLINK);
					sound.Play((uint8_t)Sounds::CHEER_LOT);
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

			cheers.Start(cheerLevel);
		}
	}

	leverPulled = false;
	leverTimer.in(LEVERANIMDELAY, showAnimLever);
}

/**
 * Called when the lever is pulled.
 */
void bounceReels()
{
	updateTimer.cancel();
	leverTimer.cancel();
	winTimer.cancel();
	game.BounceReelsBack();

	if(!leverPulled && game.totalSpins >= MAXSPINSTOWIN - SHOWREMAINING) {
		char str[7];
		if(game.totalSpins < MAXSPINSTOWIN - REMAINWARNING) {
			sprintf(str, "%d <<", MAXSPINSTOWIN - game.totalSpins);
			sound.Play((uint8_t)Sounds::SPIN_START);
		} else {
			sprintf(str, "%d !!", MAXSPINSTOWIN - game.totalSpins);
		}
		display.scroll(str);
		delay(PULLWAIT);
	}

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

		if(game.totalSpins >= MAXSPINSTOWIN - REMAINWARNING) {
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
		if(slotsMain.startLever.isReleased()) {
			display.clearAll();
			slotsMain.Restart();
			waitingForRestart = false;
		}
	} else {
		if(!game.spinning) {

			if(startLever.isPressed()) {
				bounceReels();
			} else if(startLever.isReleased()) {
				if(game.totalSpins >= MAXSPINSTOWIN - SHOWREMAINING) {
					updateDisplay(NULL);
				}
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
				} else if(decreaseBet.isPressed()) {
					game.ChangeBet(-1);
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
	feeder.Setup(servoPin);
	feeder.Return();
	ledMatrix.setup();
	display.setup();
	display.scroll(" Wait");
	cheers.Setup();
	updateTimer.every(UPDATEBET, updateBet);
	locks.Setup();
	game.Setup(startCoins);

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
	showWinSymbol = true;
	displayUpdated = false;
	waitingForRestart = false;

	// Re-creates timers

	updateTimer = timer_create_default();
	displayTimer = timer_create_default();
	leverTimer = timer_create_default();
	winTimer = timer_create_default();

	Serial.println();
	Serial.println("Slots game restarted.");
	Serial.println();

	// Initialize pins

	ioSetup();

	// Sets up objects

	feeder.Return();
	display.scroll(" Wait");
	updateTimer.every(UPDATEBET, updateBet);
	game.Setup(startCoins);

	// Perform a first home spin

	game.StartSpin(true);
}

void SlotsMain::Loop()
{
	updateTimer.tick();
	displayTimer.tick();
	leverTimer.tick();
	winTimer.tick();

	inputLoop();
	feeder.Loop();
	display.loop();
	if(game.Loop()) {
		endSpin();
	}
	nLocked = locks.Loop(
		!(game.spinning || leverPulled),
		!(firstSpin || game.spinPayoff || reelsLocked || game.nCoins <= 9),
		game.currentBet
	);
	cheers.Loop(!game.spinning && game.spinPayoff);
}

#pragma endregion

// ------------------------------------------------------------------------- End
