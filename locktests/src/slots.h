
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// Test setup for the Locks class

#pragma once

// -------------------------------------------------------------------- Includes

#include <Arduino.h>

// ------------------------------------------------------------------- Constants

#define NREELS		    3
#define EZBTNDEBOUNCE	10		// Default debounce value for ezButtons
#define ENCODERDEBOUNCE	1000	// For encoders, in μs (microseconds)
#define MAXBET			9		// 6 to 12
#define BAUDRATE		57600	// For serial communication
#define UPDATEBET		80		// Rate of display update timer
#define LOCKBLINK		400		// Blink frequency for lock LEDs
#define LOCKBLINKON		2		// Time on for lock LEDs
#define INITIALBET		3		// Initial bet

// Arduino Nano / UNO pins

static const uint8_t lockButtonPins[NREELS] = {8, 9, 10};
static const uint8_t lockLEDPins[NREELS] = {4, 5, 6};
static const uint8_t increaseBetPin = 2;
static const uint8_t decreaseBetPin = 3;


// ------------------------------------------------------------------------- End
