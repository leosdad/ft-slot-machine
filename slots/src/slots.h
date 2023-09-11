
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#pragma once

// -------------------------------------------------------------------- Includes

#include <Arduino.h>

// ----------------------------------------------------------------- Debug flags

#define DEBUGINFO		true	// Send spin debug info to the serial port
#define LOCKDEBUGINFO	false	// Send debug information about the lock state
#define SPEEDUP			true	// Remove extra reel spins and slow speed
#define SIMULATE		true	// Simulate gameplay without actually moving anything
#define CALIBRATE		false	// Set to true for reel speed calibration

// --------------------------------------------------------------------- Defines

// Gameplay constants

#define STARTCOINS		100		// Number of coins upon when game start
#define MAXCOINS		9999	// Up to 16384
#define MINLOCKCOINS	9		// Minimum coins for lock feature
#define MAXCOINDIGITS	4		// Because MAXCOINS is 999
#define MAXBET			9		// 6 to 12
#define NPAYLINES       1		// 1 to 3
#define PAYMULTIPLIER	1		// Default payoff multiplier
#define INITIALBET		3		// Initial bet
#define NCOMBINATIONS	18		// Number of payoff combinations
// #define MAXBALLS		6		// Up to 7
// #define BALLVALUE	200		// How much is needed to retrieve a ball

// Software constants

#define MX_BRIGHTNESS	1		// LED matrix brightness (0-15; zero emits light)
#define MX_TEXTPOS		9		// Offset for displaying text
#define MX_NUMPOS		12		// Offset for displaying numbers

// Time and timer constants

#define BAUDRATE		57600	// For serial communication
#define UPDATEBET		80		// Rate of display update timer
#define LOCKBLINK		300		// Blink frequency for lock LEDs
#define LOCKBLINKON		2		// Time on for lock LEDs
#define LOCKLEDFREQ		2		// lock LEDS PWM frequency
#define LOCKLEDMOD		8		// Time divider for lock LEDs
#define SIMULATETICKS	50		// Counter used in simulation mode
#define EZBTNDEBOUNCE	10		// Default debounce value for ezButtons
#define ENCODERDEBOUNCE	1000	// For encoders, in μs (microseconds)
#define DISPLAYTIME		2000	// Time for showing display messages
#define JACKPOTBLINK	300		// Blink frequency for jackpot LEDs
#define LEVERANIMDELAY	8000	// Time before lever animation
#define LEVERANIMRATE	50		// Lever animation timer frame rate
#define WINTOGGLE		600		// Period to toggle win symbol state
#define WRAPDELAY		30		// Delay for "game over" scrolling text

// Cheering constants

#define CHEER_MIN_BRT	10		// Minimum brightness
#define CHEER_WIN_BRT	255		// Maximum brightness for a win
#define CHEER_DRAW_BRT	50		// Medium brightness for a draw
#define CHEER_WIN_MS	500		// Fading time when cheering a win
#define CHEER_DRAW_MS	800		// Fading time when cheering a draw
#define CHEER_OUT_MS	500		// Fade out time
#define CHEER_LOT_MS	120		// Blinking frequency when cheering a lot
#define CHEER_LOT_ON	3000	// Period to keep LEDs on when cheering a lot
#define CHEER_LOT_RPT	1		// Cheer-a-lot repetitions
#define CHEER_WIN_RPT	10		// Win repetitions
#define CHEER_DRAW_RPT	5		// Draw repetitions

// Model-dependent constants, cannot be changed

#define NREELS		    3		// Number of reels
#define NSYMBOLS	    8		// Number of different symbols
#define NREELSYMBOLS    12		// Number of symbols per reel
#define MATRIXMODULELEN	4		// Number of modules in LED matrix
#define HOMEOFFSET		34		// Common position offset for home position

// ----------------------------------------------------------- Structs and enums

enum class SpecialFeatures {
	NONE = 0,
	SWEEPSTAKES,
	BONUS,
	JACKPOT,
};

/**
 * Defines winning combinations, payoff values and special features.
 */
struct payoffItem {
	uint8_t symbol[NREELS];
	uint16_t payoff;
	SpecialFeatures feature;
};

// ------------------------------------------------------------------- Constants

// Speed for each motor. Similar motors may behave differently, mainly at slow speeds.

#if !SPEEDUP
static const uint8_t normalSpeeds[NREELS] = {70, 90, 83};
static const uint8_t slowSpeeds[NREELS] = {45, 55, 52};
#else
static const uint8_t normalSpeeds[NREELS] = {90, 110, 100};
static const uint8_t slowSpeeds[NREELS] = {90, 110, 100};
#endif

/**
 * Encoder steps needed for each reel position. Each number of steps
 * is the previous one minus 8.5, rounded up. Derived experimentally.
 */
static const int stepOffsets[] = {0, 93, 85, 76, 68, 59, 51, 42, 34, 25, 17, 8};

/**
 * Reel composition
 */
static const int reelComposition[NREELS][NREELSYMBOLS] = {
	{1, 5, 2, 6, 8, 3, 8, 4, 2, 4, 2, 3},
	{1, 5, 4, 2, 6, 2, 3, 5, 8, 5, 6, 8},
	{1, 3, 8, 2, 8, 7, 5, 4, 5, 7, 2, 5}
};

static const uint8_t payoffMultiplier = PAYMULTIPLIER;

/**
 * 0 (zero) here means any symbol.
 */
static constexpr payoffItem payoffTable[NCOMBINATIONS] = {
	{{1, 1, 1}, 180, SpecialFeatures::NONE},
	{{3, 3, 3},  85, SpecialFeatures::JACKPOT},
	{{4, 4, 4},  85, SpecialFeatures::JACKPOT},
	{{5, 5, 3},  60, SpecialFeatures::BONUS},
	{{8, 8, 3},  45, SpecialFeatures::NONE},
	{{2, 2, 3},  30, SpecialFeatures::NONE},
	{{1, 1, 0},  25, SpecialFeatures::SWEEPSTAKES},
	{{8, 8, 8},  22, SpecialFeatures::NONE},
	{{5, 5, 5},  20, SpecialFeatures::BONUS},
	{{2, 2, 2},  15, SpecialFeatures::NONE},
	{{0, 1, 1},  15, SpecialFeatures::SWEEPSTAKES},
	{{1, 0, 1},  15, SpecialFeatures::SWEEPSTAKES},
	{{0, 3, 3},  15, SpecialFeatures::NONE},
	{{0, 3, 1},  15, SpecialFeatures::NONE},
	{{0, 4, 4},  15, SpecialFeatures::NONE},
	{{6, 6, 0},  12, SpecialFeatures::NONE},
	{{3, 3, 0},  12, SpecialFeatures::NONE},
	{{0, 0, 3},   1, SpecialFeatures::NONE},
};

// ---------------------------------------------------------------- Arduino pins

// Input pins

static const uint8_t homeSensorPins[NREELS] = {41, 42, 43};
static const uint8_t lockButtonPins[NREELS] = {35, 37, 39};
static const uint8_t encoderPins[NREELS] = {33, 31, 29};
static const uint8_t leverButtonPin = 49;
static const uint8_t increaseBetPin = 48;
static const uint8_t decreaseBetPin = 50;

// Output pins

static const uint8_t motorOutPins[NREELS][2] = { {12, 11}, {10, 9}, {8, 7} };
static const uint8_t lockLEDPins[NREELS] = {34, 36, 38};
static const uint8_t servoPin = 44;

static const uint8_t signalLED1Pin = 6;
static const uint8_t signalLED1Gnd = 5;
static const uint8_t signalLED2Pin = 3;
static const uint8_t signalLED2Gnd = 2;

static const uint8_t leftLedPin = signalLED1Pin;
static const uint8_t rightLedPin = signalLED2Pin;

// ------------------------------------------------------------------------- End
