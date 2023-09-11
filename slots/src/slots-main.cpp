
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include <arduino-timer.h>
#include "drivers/led-matrix.h"
#include "drivers/pwm-micros.h"
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

auto updateTimer = timer_create_default();
auto displayTimer = timer_create_default();
auto pullTimer = timer_create_default();
auto winTimer = timer_create_default();

// Global variables

extern SlotsMain slotsMain;
uint8_t lastBet = 255;
uint8_t lastCoins = STARTCOINS;
bool firstSpin = true;
bool reelsLocked = false;
uint8_t leverFrame = 0;
bool showWinSymbol = true;
bool displayUpdated = false;
bool waitingForRestart = false;

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

/**
 * Displays a small animated lever.
 */
bool showPull(void *)
{
	char ch;

	if(leverFrame < 10) {
		ch = '\x1a' + (leverFrame < 5 ? leverFrame : 9 - leverFrame);
		pullTimer.in(LEVERANIMRATE, showPull);
		leverFrame++;
	} else if(leverFrame == 11) {
		ch = ' ';
		pullTimer.in(500, showPull);
		leverFrame++;
	} else {
		pullTimer.in(LEVERANIMDELAY, showPull);
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
		pullTimer.cancel();
		updateDisplay(NULL);
		lastBet = game.currentBet;
		pullTimer.in(LEVERANIMDELAY, showPull);
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
	static const char* feats[] = {"", "Sweep", "Bonus", "Jckpot!"};

	if(firstSpin) {
		display.scroll("Start");
	} else {

		if(game.nCoins == 0) {
			waitingForRestart = true;
			game.playing = false;
			display.clear();
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

			CheerLevel cheerLevel;

			if(game.lastFeature > SpecialFeatures::NONE) {
				displayUpdated = false;
				cheerLevel = CheerLevel::BIG_WIN;
				display.showCentered(feats[(uint16_t)game.lastFeature]);
				displayTimer.in(DISPLAYTIME, updateDisplay);
				if(game.lastFeature == SpecialFeatures::JACKPOT) {
					display.blink(true, JACKPOTBLINK);
				}
			} else if(game.nCoins > lastCoins) {
				cheerLevel = CheerLevel::WIN;
			} else if(game.nCoins == lastCoins) {
				cheerLevel = CheerLevel::DRAW;
			} else {	// game.nCoins < lastCoins
				cheerLevel = CheerLevel::NONE;
			}

			cheers.Start(cheerLevel);
		}
	}

	pullTimer.in(LEVERANIMDELAY, showPull);
}

/**
 * Called when a new spin is about to start.
 */
void spin()
{
	cheers.Stop();
	pullTimer.cancel();
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

		display.scroll("Spin ");

		#if !SPEEDUP
		delay(500);
		#endif
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
			display.clear();
			slotsMain.Restart();
			waitingForRestart = false;
		}
	} else {
		if(!game.spinning) {
			if(startLever.isReleased()) {
				firstSpin = false;
				spin();
			} else if(increaseBet.isPressed()) {
				game.ChangeBet(1);
			} else if(decreaseBet.isPressed()) {
				game.ChangeBet(-1);
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

	ledMatrix.setup();
	display.setup();
	display.scroll(" Wait");
	cheers.Setup();
	updateTimer.every(UPDATEBET, updateBet);
	locks.Setup();
	game.Setup();

	// Perform a first (home) spin

	game.StartSpin(true);
}


void SlotsMain::Restart()
{
	// Resets variable values

	lastBet = 255;
	lastCoins = STARTCOINS;
	firstSpin = true;
	reelsLocked = false;
	leverFrame = 0;
	showWinSymbol = true;
	displayUpdated = false;
	waitingForRestart = false;

	// Re-creates timers

	updateTimer = timer_create_default();
	displayTimer = timer_create_default();
	pullTimer = timer_create_default();
	winTimer = timer_create_default();

	Serial.println("Slots game restarted.");
	Serial.println();

	// Initialize pins

	ioSetup();

	display.scroll(" Wait");
	updateTimer.every(UPDATEBET, updateBet);
	game.Setup();

	// Perform a first home spin

	game.StartSpin(true);
}

void SlotsMain::Loop()
{
	updateTimer.tick();
	displayTimer.tick();
	pullTimer.tick();
	winTimer.tick();

	inputLoop();
	display.loop();
	if(game.Loop()) {
		endSpin();
	}
	locks.Loop(
		!game.spinning,
		!(firstSpin || game.spinPayoff || reelsLocked || game.nCoins <= 9),
		game.currentBet
	);
	cheers.Loop(!game.spinning && game.spinPayoff);
}

#pragma endregion

// ------------------------------------------------------------------------- End
