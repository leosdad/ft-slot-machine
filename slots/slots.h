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
	{0, 4, 1, 5, 7, 2, 7, 3, 1, 3, 1, 2},
	{0, 4, 3, 1, 5, 1, 2, 4, 7, 4, 5, 7},
	{0, 2, 7, 1, 7, 6, 4, 3, 4, 6, 1, 4}
};

/**
 * Each line is: reel 0, reel 1, reel 2, payoff. -1 on a reel means "any".
 */
const int16_t payoffs[NPAYOFFS][NREELS + 1] = {
	{0, 0, 0, 516},
	{2, 2, 2, 258},
	{3, 3, 3, 258},
	{4, 4, 2, 174},
	{7, 7, 2, 129},
	{1, 1, 2, 87},
	{7, 7, 7, 66},
	{4, 4, 4, 60},
	{1, 1, 1, 42},
	{0, 0, -1, 42},
	{-1, 0, 0, 42},
	{0, -1, 0, 42},
	{-1, 2, 2, 42},
	{0, 2, -1, 42},
	{-1, 2, 2, 42},
	{4, 2, -1, 42},
	{5, 5, -1, 21},
	{2, 2, -1, 21},
	{2, 0, -1, 21},
	{7, 7, -1, 12},
	{-1, 7, 7, 12},
	{-1, -1, 2, 3},
};

#endif	// SLOTS_H

// ------------------------------------------------------------------------- End
