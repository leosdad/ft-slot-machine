// -----------------------------------------------------------------------------

// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -----------------------------------------------------------------------------

#ifndef SLOTS_H
#define SLOTS_H

// -------------------------------------------------------------------- Includes

#include <Arduino.h>

// ----------------------------------------------------------------- Debug flags

// Define for faster tests
#define SPEEDUP true

// Define for reel speed calibration
// #define CALIBRATE

// Define to simulate gameplay without actually moving anything
// #define SIMULATE

// Define to show debug data on the OLED display
// #define SHOWDEBUGDATA

// --------------------------------------------------------------------- Defines

#define NREELS		    3
#define NSYMBOLS	    8
#define NREELSYMBOLS    12
#define NPAYLINES       2
#define BLINKINTERVAL	200

// ---------------------------------------------------------------- Arduino pins

// Motor pins

const uint8_t motor1Out[2] = {46, 12};
const uint8_t motor2Out[2] = {11, 10};
const uint8_t motor3Out[2] = {9, 8};

const uint8_t motorOut[NREELS][2] = {
	{46, 12},
	{11, 10},
	{9, 8}
};

// Input pins

const uint8_t encoder[NREELS] = {28, 30, 32};
const uint8_t reelButtonPin[NREELS] = {35, 37, 39};
const uint8_t positionSensor[NREELS] = {29, 31, 33};
const uint8_t leverButtonPin = 53;
const uint8_t increaseBetPin = 52;
const uint8_t decreaseBetPin = 50;

// Output pins

const uint8_t lockLED[NREELS] = {34, 36, 38};
const uint8_t redLED1[2] = {2, 3};
const uint8_t redLED2[2] = {6, 7};
const uint8_t Servo = 44;

// ------------------------------------------------------------- Other constants

// Used with encoders. In μs (microseconds)
const unsigned long debouncePeriod = 1000;

// Common position offset for home position
const uint16_t homeOffset = 34;

// Speed for each motor. Similar motors may behave differently at slow speeds.
const uint8_t reelSpeed[NREELS] = {70, 90, 83};

// Number of initial coins
const uint16_t startCoins = 3;

const uint16_t maxCoins = 9;

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

#endif	// SLOTS_H

// ------------------------------------------------------------------------- End
