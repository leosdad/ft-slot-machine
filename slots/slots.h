// -----------------------------------------------------------------------------

// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -----------------------------------------------------------------------------

#ifndef SLOTS_H
#define SLOTS_H

#include <Arduino.h>

#define NREELS	3

// Arduino Mega input pins

const uint8_t decreaseBet = 50;
const uint8_t increaseBet = 52;
const uint8_t leverButton = 53;
const uint8_t encoder[] = { 28, 30, 32 };
const uint8_t posSensor[] = { 29, 31, 33 };
const uint8_t toggle[] = {35, 37, 39};

// Arduino Mega output pins

const uint8_t motor1Out[] = {12, 46};
const uint8_t motor2Out[] = {10, 11};
const uint8_t motor3Out[] = {8, 9};
const uint8_t redLED1[] = {4, 5};
const uint8_t redLED2[] = {6, 7};
const uint8_t lockLED[] = { 34, 36, 38 };
const uint8_t Servo = 44;

// Other constants

const unsigned long debouncePeriod = 1000;	// Microseconds

// const uint16_t initialSteps = 260;			// Steps after sensor is triggered
// const uint16_t speedDecrement = 5;			// Speed decrement between each reduction
// const uint16_t finalSteps = 30;				// Steps between each reduction
// const uint8_t maxSpeed = 180;				// Maximum speed
// const uint8_t normalSpeed = 100;			// Normal speed
// const uint8_t minSpeed = 50;				// Minimum speed
// const uint8_t extraRotations[] = {1, 8, 18};

// const uint16_t speedDecrement = 1;			// Speed decrement between each reduction
// // const uint16_t finalSteps[] = {100, 100, 100};				// Steps after sensor is triggered
// const uint16_t finalSteps[] = {0, 0, 0};				// Steps after sensor is triggered
// // const uint8_t maxSpeed = 80;				// Maximum speed
// const uint8_t normalSpeed[] = {80, 80, 80};			// Normal speed
// const uint8_t minSpeed[] = {50, 50, 50};				// Minimum speed
// const uint8_t extraRotations[] = {0, 0, 0};

#endif	// SLOTS_H
