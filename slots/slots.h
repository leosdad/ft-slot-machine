// -----------------------------------------------------------------------------

// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -----------------------------------------------------------------------------

#ifndef SLOTS_H
#define SLOTS_H

#include <Arduino.h>

#define NREELS	3

// Arduino pins

const uint8_t leverButton = 8;
const uint8_t coinIncrementButton = 12;
const uint8_t encoder[] = { 2, 4, 7 };
const uint8_t motor1Ports[] = {3, 5};
const uint8_t motor2Ports[] = {6, 9};
const uint8_t motor3Ports[] = {10, 11};
const uint8_t posSensor[] = { A0, A1, A2 };
const uint8_t jackpotLED = 13;
const uint8_t lockLED[] = { A3, A6, A7 };

// Other constants

const unsigned long debouncePeriod = 1000;	// Microseconds

// const uint16_t initialSteps = 260;			// Steps after sensor is triggered
// const uint16_t speedDecrement = 5;			// Speed decrement between each reduction
// const uint16_t finalSteps = 30;				// Steps between each reduction
// const uint8_t maxSpeed = 180;				// Maximum speed
// const uint8_t normalSpeed = 100;			// Normal speed
// const uint8_t minSpeed = 50;				// Minimum speed
// const uint8_t extraRotations[] = {1, 8, 18};

const uint16_t speedDecrement = 1;			// Speed decrement between each reduction
// const uint16_t finalSteps[] = {100, 100, 100};				// Steps after sensor is triggered
const uint16_t finalSteps[] = {0, 0, 0};				// Steps after sensor is triggered
// const uint8_t maxSpeed = 80;				// Maximum speed
const uint8_t normalSpeed[] = {80, 80, 80};			// Normal speed
const uint8_t minSpeed[] = {50, 50, 50};				// Minimum speed
const uint8_t extraRotations[] = {0, 0, 0};

#endif	// SLOTS_H
