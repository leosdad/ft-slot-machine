
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#pragma once

// -------------------------------------------------------------------- Includes

#include <Arduino.h>

// ----------------------------------------------------------------- Debug flags

#define DEBUGINFO		true	// Send spin debug info to the serial port
#define DEBUGPAYOFFS	false	// Easy-to-obtain payoffs, awards for testing
#define SPEEDUP			false	// Remove extra reel spins, rotate reels faster
#define SIMULATE		false	// Simulate gameplay without moving anything
#define LOCKDEBUGINFO	false	// Send debug information about the lock state
#define CALIBRATE		false	// Set to true for reel speed calibration
#define DEBUGREELS		false	// Show info about reels state and rotation

// --------------------------------------------------------------------- Defines

// Gameplay constants

#if DEBUGPAYOFFS
#define STARTCOINS		25		// Default number of coins upon game start
#define MAXSTARTCOINS	29		// Maximum mumber of start coins
#define DEFSPINSLEFT	12		// Default remaining spins per game
#define VICTORYPOINTS	30		// Coins needed to win a ball (victory)
#else
#define STARTCOINS		100		// Default number of coins upon game start
#define MAXSTARTCOINS	150		// Maximum mumber of start coins
#define DEFSPINSLEFT	50		// Default remaining spins per game
#define VICTORYPOINTS	400		// Coins needed to win a ball (victory)
#endif

#define SHOWREMAINING	10		// Spins to start displaying remaining spins
#define REMAINWARNING	5		// Spins to start displaying game end warning
#define DOUBLESPINS		5		// Spins during which double pay is enabled
#define BONUSSPINS		5		// Extra spins when a bonus is awarded
#define LOCKPENALTYDIV	150		// Divider used to calculate lock penalty

// Gameplay constants that won't change

#define MAXCOINS		9999	// Up to 16384, but display is 4-digit only
#define MAXCOINDIGITS	4		// Because MAXCOINS is 9999
#define MINLOCKCOINS	9		// Minimum coins for lock feature
#define MAXBET			9		// 6 to 12
#define NPAYLINES       1		// 1 to 3
#define INITIALBET		3		// Default bet upon game start
#define NCOMBINATIONS	18		// Number of payoff combinations

// Software constants

#define VOLUME			14		// Sound volume (0-15)
#define MX_BRIGHTNESS	3		// LED matrix brightness (0-15; zero emits light)
#define MX_TEXTPOS		9		// Offset for displaying text
#define MX_NUMPOS		12		// Offset for displaying numbers

// Time and timer constants

#define BAUDRATE		57600	// For serial communication
#define UPDATEBET		80		// Rate of display update timer
#define LOCKBLINK		300		// Blink frequency for lock LEDs
#define SIMULATETICKS	50		// Counter used in simulation mode
#define EZBTNDEBOUNCE	10		// Default debounce value for ezButtons
#define ENCODERDEBOUNCE	1000	// For encoders, in μs (microseconds)
#define HOMEDEBOUNCE	1000	// For home sensors, in μs (microseconds)
#define CHEERENDTIME	2000	// Time to end cheering message
#define TOPSCOREBLINK	300		// Blink frequency for top score LEDs
#define LEVERANIMDELAY	8123	// Time before lever animation
#define LEVERANIMRATE	50		// Lever animation timer frame rate
#define LEVERWAIT		500		// Wait time before reversing lever animation
#define LIFETOGGLE		180		// Period to toggle life indicator
#define WRAPDELAY		30		// Delay for "game over" scrolling text
#define BOUNCETIME		5		// Delay between rotate and brake
#define REMAINSTART		800		// Time to show remaining spins
#define REMAINRESET		1000	// Time to go back to regular display
#define REMAINREPEAT	5017	// Time to repeat show remaining spins

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
#define CHEER_VICT_TOG	1000	// Blinking frequency for victory

// Model-dependent constants, cannot be changed

#define NREELS		    3		// Number of reels
#define NSYMBOLS	    8		// Number of different symbols
#define NREELSYMBOLS    12		// Number of symbols per reel
#define MATRIXMODULELEN	4		// Number of modules in LED matrix
#define HOMEOFFSET		34		// Common position offset for home position

// ----------------------------------------------------------- Structs and enums

enum class Awards {
	NONE = 0,
	DOUBLE,
	BONUS,
	TOPSCORE,
};

enum class GameResult {
	NONE = 0,
	VICTORY,
	NO_COINS_LEFT,
	NO_SPINS_LEFT,
};

/**
 * Defines winning combinations, payoff values and special awards.
 */
struct payoffItem {
	uint8_t symbol[NREELS];
	uint16_t payoff;
	Awards award;
};

enum class Sounds {
	HELLO = 1,
	GAME_LOST,
	GAME_WON,
	END_IS_NEAR,
	CHEER_WIN,
	CHEER_DRAW,
	CHEER_BONUS,
	CHEER_TOPSCORE
};

// ------------------------------------------------------------------- Constants

// Speed for each motor. Similar motors may behave differently, mainly at slow speeds.

#if !SPEEDUP
static const uint8_t normalSpeeds[NREELS] = {90, 110, 100};
static const uint8_t slowSpeeds[NREELS] = {55, 75, 58};
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

/**
 * 0 (zero) below means any symbol.
 */
static constexpr payoffItem payoffTable[NCOMBINATIONS] = {

#if !DEBUGPAYOFFS

	// These should match the spreadsheet and the printed table.

	{{1, 1, 1}, 120, Awards::TOPSCORE},
	{{3, 3, 3},  85, Awards::TOPSCORE},
	{{4, 4, 4},  85, Awards::TOPSCORE},
	{{5, 5, 3},  60, Awards::DOUBLE},
	{{8, 8, 3},  45, Awards::NONE},
	{{2, 2, 3},  30, Awards::NONE},
	{{8, 8, 8},  22, Awards::NONE},
	{{5, 5, 5},  20, Awards::DOUBLE},
	{{2, 2, 2},  15, Awards::NONE},
	{{1, 1, 0},  15, Awards::BONUS},
	{{0, 1, 1},  15, Awards::BONUS},
	{{1, 0, 1},  15, Awards::BONUS},
	{{0, 3, 3},  15, Awards::NONE},
	{{0, 3, 1},  15, Awards::NONE},
	{{0, 4, 4},  15, Awards::NONE},
	{{6, 6, 0},  12, Awards::NONE},
	{{3, 3, 0},  12, Awards::NONE},
	{{0, 0, 3},   1, Awards::NONE},

#else

	// These are for testing purposes.

	{{1, 1, 1}, 120, Awards::TOPSCORE},
	{{3, 3, 3},  85, Awards::TOPSCORE},
	{{4, 4, 4},  85, Awards::TOPSCORE},
	{{5, 5, 3},  60, Awards::DOUBLE},
	{{8, 8, 3},  45, Awards::NONE},
	{{2, 2, 3},  30, Awards::NONE},
	{{8, 8, 8},  22, Awards::NONE},
	{{5, 5, 5},  20, Awards::DOUBLE},
	{{6, 0, 0},   1, Awards::TOPSCORE},	// Orange
	{{2, 0, 0},   1, Awards::BONUS},	// Bananas
	{{0, 2, 0},   1, Awards::BONUS},
	{{0, 0, 2},   1, Awards::BONUS},
	{{1, 0, 0},   2, Awards::DOUBLE},	// Sevens
	{{0, 1, 0},   2, Awards::DOUBLE},
	{{0, 0, 1},   2, Awards::DOUBLE},
	{{3, 0, 0},   2, Awards::NONE},		// Cherries
	{{0, 3, 0},   2, Awards::NONE},
	{{0, 0, 3},   2, Awards::NONE},

#endif

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
