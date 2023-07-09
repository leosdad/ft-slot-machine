// -----------------------------------------------------------------------------

// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -----------------------------------------------------------------------------

#ifndef SLOTS_H
#define SLOTS_H

// -------------------------------------------------------------------- Includes

#include <Arduino.h>

// ------------------------------------------------------------------ Test flags

// Set to true for faster tests
#define SPEEDUP		 true

// Set to true for reel speed calibration
#define CALIBRATE	 false

// Set to true to simulate gameplay without actually moving anything
#define SIMULATE	 false

// --------------------------------------------------------------------- Defines

#define NREELS		 3
#define NSYMBOLS	 8
#define NREELSYMBOLS 12
#define NPAYOFFS	 22

// ---------------------------------------------------------------- Arduino pins

// Input pins

const uint8_t encoder[] = {28, 30, 32};
const uint8_t reelButtonPin[] = {35, 37, 39};
const uint8_t positionSensor[] = {29, 31, 33};
const uint8_t leverButtonPin = 53;
const uint8_t increaseBetPin = 52;
const uint8_t decreaseBetPin = 50;

// Output pins

const uint8_t lockLED[] = {34, 36, 38};
const uint8_t redLED1[] = {2, 3};
const uint8_t redLED2[] = {6, 7};
const uint8_t Servo = 44;

const uint8_t motor1Out[] = {46, 12};
const uint8_t motor2Out[] = {11, 10};
const uint8_t motor3Out[] = {9, 8};

// ------------------------------------------------------------- Other constants

// In μs (microseconds)
const unsigned long debouncePeriod = 1000;

// Common offset for home position
const uint16_t homeOffset = 34;

// Speed for each motor. Similar motors may behave differently at slow speeds.
const uint8_t reelSpeed[] = {70, 90, 83};

/**
 * Encoder steps needed for each reel position. Each number of steps
 * is the previous one minus 8.5, rounded up.
 */
const int stepOffsets[] = {0, 93, 85, 76, 68, 59, 51, 42, 34, 25, 17, 8};

/**
 * Reel composition
 */
const int reels[NREELS][NREELSYMBOLS] = {
	{1, 5, 2, 6, 8, 3, 8, 4, 2, 4, 2, 3},
	{1, 5, 4, 2, 6, 2, 3, 5, 8, 5, 6, 8},
	{1, 3, 8, 2, 8, 7, 5, 4, 5, 7, 2, 5}
};

/**
 * Each line is: reel 0, reel 1, reel 2, payoff. -1 on a reel means "any".
 */
const int16_t payoffTable[NPAYOFFS][NREELS + 1] = {
	{1, 1, 1, 172},
	{3, 3, 3, 86},
	{4, 4, 4, 86},
	{5, 5, 3, 58},
	{8, 8, 3, 43},
	{2, 2, 3, 29},
	{8, 8, 8, 22},
	{5, 5, 5, 20},
	{2, 2, 2, 14},
	{1, 1, 0, 14},
	{0, 1, 1, 14},
	{1, 0, 1, 14},
	{0, 3, 3, 14},
	{0, 3, 1, 14},
	{0, 4, 4, 14},
	{5, 3, 0, 14},
	{6, 6, 0, 7},
	{3, 3, 0, 7},
	{3, 1, 0, 7},
	{8, 8, 0, 4},
	{0, 8, 8, 4},
	{0, 0, 3, 1},
};

#endif	// SLOTS_H

// ------------------------------------------------------------------------- End
