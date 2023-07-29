// -----------------------------------------------------------------------------

// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -----------------------------------------------------------------------------

#ifndef SLOTS_H
#define SLOTS_H

// -------------------------------------------------------------------- Includes

#include <Arduino.h>

// --------------------------------------------------------------------- Defines

// Debug flags

#define DEBUGINFO		true	// Show debug data on the OLED display
#define SPEEDUP			true	// Set to true for faster spins
#define SIMULATE		false	// Set to true to simulate gameplay with no spins
#define CALIBRATE		false	// Set to true for reel speed calibration

// Game constants

#define MAXBET			9		// 6 to 12
#define NPAYLINES       2		// 1 to 3
#define STARTCOINS		200
#define PAYMULTIPLIER	1
#define INITIALBET		3
#define NCOMBINATIONS	22		// Number of payoff combinations

// Time constants

#define BLINKINTERVAL	350
#define SIMULATE_DELAY	50
#define EZBTNDEBOUNCE	50

// Hardware-dependent constants, cannot be changed

#define NREELS		    3
#define NSYMBOLS	    8
#define NREELSYMBOLS    12

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

// ---------------------------------------------------------------- Arduino pins

// Motor pins

static const uint8_t motor1Out[2] = {46, 12};
static const uint8_t motor2Out[2] = {11, 10};
static const uint8_t motor3Out[2] = {9, 8};

static const uint8_t motorOut[NREELS][2] = {
	{46, 12},
	{11, 10},
	{9, 8}
};

// Input pins

static const uint8_t homeSensorPin[NREELS] = {29, 31, 33};
static const uint8_t lockButtonPin[NREELS] = {35, 37, 39};
static const uint8_t encoderPin[NREELS] = {28, 30, 32};

static const uint8_t leverButtonPin = 53;
static const uint8_t increaseBetPin = 52;
static const uint8_t decreaseBetPin = 50;

// Output pins

static const uint8_t lockLEDPin[NREELS] = {34, 36, 38};
static const uint8_t signalLED1[2] = {2, 3};	// 0 V, 9 V
static const uint8_t signalLED2[2] = {6, 7};	// 0 V, 9 V
static const uint8_t Servo = 44;

// ------------------------------------------------------------------------- End

#endif	// SLOTS_H
