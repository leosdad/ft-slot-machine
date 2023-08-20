// -----------------------------------------------------------------------------

// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -----------------------------------------------------------------------------

#ifndef SLOTS_CONSTANTS_H
#define SLOTS_CONSTANTS_H

// -------------------------------------------------------------------- Includes

#include <Arduino.h>

// --------------------------------------------------------------------- Defines

// Debug flags

#define DEBUGINFO		false	// Show debug data on the OLED display
#define SPEEDUP			true	// Set to true for faster spins
#define SIMULATE		false	// Set to true to simulate gameplay with no spins
#define CALIBRATE		false	// Set to true for reel speed calibration

// Game constants

#define STARTCOINS		100		// Player coins when game starts
#define MAXCOINS		9999	// Up to 16384
#define MAXBET			9		// 6 to 12
#define NPAYLINES       1		// 1 to 3
#define BALLVALUE		101		// How much is needed to retrieve a ball
#define MAXBALLS		6		// Up to 7
#define PAYMULTIPLIER	1		// Default payoff multiplier
#define INITIALBET		3		// Initial bet
#define NCOMBINATIONS	22		// Number of payoff combinations

// Time and hardware constants

#define LOCKBLINKON		80
#define LOCKBLINKOFF	500
#define LOCKBLINKVALUE	15
#define CHEERNORMALMS	550
#define CHEERALOTMS		120
#define SIMULATEDELAY	50
#define EZBTNDEBOUNCE	10

// ft model-dependent constants, cannot be changed

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

static const bool speedUp = SPEEDUP;		// Remove extra reel spins
static const bool simulate = SIMULATE;		// Simulate gameplay without actually moving anything
static const bool calibrate = CALIBRATE;	// Used for reel speed calibration

// Speed for each motor. Similar motors may behave differently at slow speeds.
static const uint8_t motorSpeed[NREELS] = {70, 90, 83};

// Used with encoders. In μs (microseconds)
static const unsigned long debouncePeriod = 1000;

// Common position offset for home position
static const uint16_t homeOffset = 34;

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

#endif	// SLOTS_CONSTANTS_H