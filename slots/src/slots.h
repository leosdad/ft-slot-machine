
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#pragma once

// -------------------------------------------------------------------- Includes

#include <Arduino.h>

// ----------------------------------------------------------------- Debug flags

#define DEBUGINFO		false	// Send spin debug info to the serial port
#define LOCKDEBUGINFO	false	// Send debug information about the lock state
#define SPEEDUP			true	// Remove extra reel spins and slow speed
#define SIMULATE		false	// Simulate gameplay without actually moving anything
#define CALIBRATE		false	// Set to true for reel speed calibration

// --------------------------------------------------------------------- Defines

// Game constants

#define STARTCOINS		100		// Player coins when game starts
#define MAXCOINS		9999	// Up to 16384
#define MAXCOINDIGITS	4		// Because MAXCOINS is 9999
#define MAXBET			9		// 6 to 12
#define NPAYLINES       1		// 1 to 3
#define PAYMULTIPLIER	1		// Default payoff multiplier
#define INITIALBET		3		// Initial bet
#define NCOMBINATIONS	22		// Number of payoff combinations
#define BALLVALUE		101		// How much is needed to retrieve a ball
#define MAXBALLS		6		// Up to 7

// Time and hardware constants

#define BAUDRATE		57600	// For serial communication
#define UPDATEBET		80		// Rate of display update timer
#define LOCKBLINK		300		// Blink frequency for lock LEDs
#define LOCKBLINKON		2		// Time on for lock LEDs
#define LOCKLEDFREQ		2		// lock LEDS PWM frequency
#define LOCKLEDMOD		8		// Time divider for lock LEDs
#define SIMULATEDELAY	50		// Delay used in simulation mode
#define EZBTNDEBOUNCE	10		// Default debounce value for ezButtons
#define HOMEOFFSET		34		// Common position offset for home position
#define ENCODERDEBOUNCE	1000	// For encoders, in μs (microseconds)
#define MX_BRIGHTNESS   0		// LED matrix brightness
#define MX_TEXTPOS		9		// Offset for displaying text
#define MX_NUMPOS		12		// Offset for displaying numbers
#define CHEER_MIN		10		// Minimum brightness for cheering
#define CHEER_MAX		255		// Maximum brightness for cheering
#define CHEERMS			500		// Fading frequency when cheering
#define CHEERALOTMS		120		// Blinking frequency when cheering a lot

// Model-dependent constants, cannot be changed

#define NREELS		    3
#define NSYMBOLS	    8
#define NREELSYMBOLS    12
#define MATRIXMODULELEN	4

// ----------------------------------------------------------- Structs and enums

enum class SpecialFeatures {
	NONE = 0x00,
	JACKPOT = 0x01,
	// BONUS = 0x02,
};

/**
 * Defines winning combinations, payoff values and special features.
 */
struct payoffItem {
	uint8_t symbol[NREELS];
	uint16_t payoff;
	SpecialFeatures features;
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
	{{1, 1, 1}, 172, SpecialFeatures::NONE},
	{{3, 3, 3}, 86, SpecialFeatures::NONE},
	{{4, 4, 4}, 86, SpecialFeatures::NONE},
	{{5, 5, 3}, 58, SpecialFeatures::NONE},
	{{8, 8, 3}, 43, SpecialFeatures::NONE},
	{{2, 2, 3}, 29, SpecialFeatures::NONE},
	{{8, 8, 8}, 22, SpecialFeatures::NONE},
	{{5, 5, 5}, 20, SpecialFeatures::NONE},
	{{2, 2, 2}, 14, SpecialFeatures::NONE},
	{{1, 1, 0}, 14, SpecialFeatures::NONE},
	{{0, 1, 1}, 14, SpecialFeatures::NONE},
	{{1, 0, 1}, 14, SpecialFeatures::NONE},
	{{0, 3, 3}, 14, SpecialFeatures::NONE},
	{{0, 3, 1}, 14, SpecialFeatures::NONE},
	{{0, 4, 4}, 14, SpecialFeatures::NONE},
	{{5, 3, 0}, 14, SpecialFeatures::NONE},
	{{6, 6, 0}, 7, SpecialFeatures::NONE},
	{{3, 3, 0}, 7, SpecialFeatures::NONE},
	{{3, 1, 0}, 7, SpecialFeatures::NONE},
	{{8, 8, 0}, 4, SpecialFeatures::NONE},
	{{0, 8, 8}, 4, SpecialFeatures::NONE},
	{{0, 0, 3}, 1, SpecialFeatures::NONE},
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
