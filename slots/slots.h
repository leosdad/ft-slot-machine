// -----------------------------------------------------------------------------

// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -----------------------------------------------------------------------------

#ifndef SLOTS_H
#define SLOTS_H

// -------------------------------------------------------------------- Includes

#include <Arduino.h>
#include <ezButton.h>  // ezButton library
#include <ezOutput.h>  // ezOutput library

#include "MotorDriver.h"
#include "slots.h"
#include "src/oled-display.h"

// --------------------------------------------------------------------- Defines

#define NREELS 3

// ------------------------------------------------------------------- Constants

// Input encoders and position sensors

const uint8_t encoder[] = {28, 30, 32};
const uint8_t posSensor[] = {29, 31, 33};

// Input switches

const uint8_t decreaseBet = 50;
const uint8_t increaseBet = 52;
const uint8_t leverButton = 53;
const uint8_t toggle[] = {35, 37, 39};

// Output pins

const uint8_t lockLED[] = {34, 36, 38};
const uint8_t redLED1[] = {4, 5};
const uint8_t redLED2[] = {6, 7};
const uint8_t Servo = 44;
const uint8_t motor1Out[] = {12, 46};
const uint8_t motor2Out[] = {10, 11};
const uint8_t motor3Out[] = {8, 9};

// Other constants

const unsigned long debouncePeriod = 1000;	 // Microseconds
const uint8_t normalSpeed[] = {64, 70, 70};	 // Normal speed
const uint8_t extraRotations[] = {1, 2, 3};  // Full extra rotations after first one
// const uint8_t extraRotations[] = {0,0,0};	 // Set all to zero to speed up tests
const uint16_t homeOffset = 34;				 // Common offset for home position
const uint16_t finalSteps[] = {0, 0, 0};	 // Steps after sensor is triggered

// const uint16_t speedDecrement = 5;			 // Speed decrement between each reduction
// const uint8_t minSpeed[] = {50, 50, 50};	 // Minimum speed
// const uint8_t maxSpeed[] = {180, 180, 180};	 // Maximum speed

#endif	// SLOTS_H

// ------------------------------------------------------------------------- End
