// -----------------------------------------------------------------------------

// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -----------------------------------------------------------------------------

#ifndef SLOTS_H
#define SLOTS_H

// -------------------------------------------------------------------- Includes

#include <Arduino.h>

// --------------------------------------------------------------------- Defines

#define NREELS		 3
#define NSYMBOLS	 8
#define NREELSYMBOLS 12
#define NPAYOFFS	 12

// ------------------------------------------------------------------- Constants

// Input pins

const uint8_t encoder[] = {28, 30, 32};
const uint8_t toggle[] = {35, 37, 39};
const uint8_t posSensor[] = {29, 31, 33};
const uint8_t decreaseBet = 50;
const uint8_t increaseBet = 52;
const uint8_t leverButton = 53;

// Output pins

const uint8_t lockLED[] = {34, 36, 38};
const uint8_t redLED1[] = {2, 3};
const uint8_t redLED2[] = {6, 7};
const uint8_t Servo = 44;

// const uint8_t motor1Out[] = {12, 46};
// const uint8_t motor2Out[] = {10, 11};
// const uint8_t motor3Out[] = {8, 9};
const uint8_t motor1Out[] = {46, 12};
const uint8_t motor2Out[] = {11, 10};
const uint8_t motor3Out[] = {9, 8};

// Other constants

// In μs (microseconds)
const unsigned long debouncePeriod = 1000;

// Common offset for home position
const uint16_t homeOffset = 34;

// Normal speed. Similar motors behave differently at slow speeds
const uint8_t normalSpeed[] = {76, 80, 80};

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
	{0,  0,  0,  450},
	{2,  2,  2,  300},
	{3,  3,  3,  225},
	{4,  4,  2,  180},
	{7,  7,  2,  135},
	{1,  1,  2,  90},
	{7,  7,  7,  75},
	{4,  4,  4,  60},
	{1,  1,  1,  45},
	{5,  5, -1,  15},
	{7,  7, -1,  6},
	{5, -1, -1,  3},
};

// const char *payoffText[NPAYOFFS] = {
// 	"3 sevens!!!",
// 	"3 cherries!!",
// 	"3 watermelons!",
// 	"2 bells, cherry",
// 	"2 grapes, cherry",
// 	"2 banana, cherry",
// 	"3 grapes",
// 	"3 bells",
// 	"3 bananas",
// 	"2 oranges",
// 	"2 grapes",
// 	"1 orange",
// };

#endif	// SLOTS_H

// ------------------------------------------------------------------------- End
