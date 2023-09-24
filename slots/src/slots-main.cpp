
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include <arduino-timer.h>
#include "drivers/led-matrix.h"
#include "drivers/pwm-micros.h"
#include "drivers/ball-feeder.h"
#include "drivers/sound.h"
#include "cheering.h"
#include "display.h"
#include "game.h"
#include "locks.h"
#include "reel.h"
#include "slots-main.h"

// ------------------------------------------------------------ Global variables

// Global objects

Game game;
Locks locks;
LedMatrix ledMatrix;
Display display;
Cheering cheers;
BallFeeder feeder;
Sound sound;

auto updateTimer = timer_create_default();
auto displayTimer = timer_create_default();
auto leverTimer = timer_create_default();
auto winTimer = timer_create_default();

// Global variables

extern SlotsMain slotsMain;
uint8_t lastBet = 255;
uint8_t lastCoins = -1;
bool firstSpin = true;
bool reelsLocked = false;
bool leverPulled = false;
uint8_t leverFrame = 0;
bool showWinSymbol = true;
bool displayUpdated = false;
bool waitingForRestart = false;
uint16_t startCoins = STARTCOINS;
uint8_t nBalls = 0;

// ------------------------------------------------------------ Global functions

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

bool updateDisplay(void *)
{
	display.blink(false);
	displayUpdated = true;

	if(game.nCoins > 0) {
		if(game.currentBet == 0) {
			display.showCentered("No bet");
		} else {
			display.showBet(game.currentBet);
			display.show(game.nCoins, true);
		}
	}
	return true;
}

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

bool updateBet(void *)
{
	if(game.currentBet != lastBet) {
		if(leverFrame != 0) {
			ledMatrix.printText(" ", MX_TEXTPOS);
		}
		leverTimer.cancel();
		updateDisplay(NULL);
		lastBet = game.currentBet;
		leverTimer.in(LEVERANIMDELAY, showAnimLever);
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

	if(firstSpin) {
		display.scroll("Start");
	} else {

		if(game.nCoins == 0) {

			waitingForRestart = true;
			game.playing = false;
			startCoins = STARTCOINS;
			display.clearAll();
			sound.Play((uint8_t)Sounds::GAME_OVER);
			ledMatrix.wrapText("Game over. Pull lever to restart ... ", wrapLoop);

		} else {

			// Enforces the maximum bet value if needed
			game.ChangeBet();

			if(game.spinPayoff) {
				showWinSymbol = true;
				winTimer.every(WINTOGGLE, toggleWinSymbol);
			} else {
				winTimer.cancel();
			}

			updateDisplay(NULL);

			if(game.nCoins >= BALLVALUE) {
				nBalls++;
				// Serial.println("----------> " + String(game.nCoins));
				winTimer.cancel();
				feeder.Feed();
				waitingForRestart = true;
				game.playing = false;
				startCoins = min(MAXSTARTCOINS, STARTCOINS + (game.nCoins - BALLVALUE) / 2);
				// Serial.println("----------> " + String(startCoins));
				display.clearAll();

				char str[60];
				sprintf(str, "Game won with %d points! Pull lever to restart ... ", game.nCoins);
				ledMatrix.wrapText(str, wrapLoop);
			}

			CheerLevel cheerLevel;

			if(game.lastFeature > SpecialFeatures::NONE) {
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
				// sound.Play((uint8_t)Sounds::WAITING);
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
	leverPulled = true;
	leverTimer.cancel();
	winTimer.cancel();
	game.BounceReelsBack();
}

/**
 * Called when a new spin is about to start.
 */
void spin()
{
	cheers.Stop();
	leverTimer.cancel();
	winTimer.cancel();

	if(game.nCoins > 0) {
		if(game.currentBet == 0) {
			return;
		}
		lastCoins = game.nCoins;
		reelsLocked = false;
		for(int i = 0; i < NREELS; i++) {
			if(locks.IsLocked(i)) {
				reelsLocked = true;
			}
		}

		// display.scroll("Spin ");
		sound.Play((uint8_t)Sounds::SPINNING);

		// #if !SPEEDUP
		// delay(500);
		// #endif
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
	// TODO: lots of common code with Setup() above

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
	locks.Loop(
		!(game.spinning || leverPulled),
		!(firstSpin || game.spinPayoff || reelsLocked || game.nCoins <= 9),
		game.currentBet
	);
	cheers.Loop(!game.spinning && game.spinPayoff);
}

#pragma endregion

// ------------------------------------------------------------------------- End
