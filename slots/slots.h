// -----------------------------------------------------------------------------

// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -----------------------------------------------------------------------------

#ifndef SLOTS_H
#define SLOTS_H

#include <Arduino.h>

// Arduino pins

const uint8_t leverButton = 8;
const uint8_t coinIncrementButton = 12;

const uint8_t encoder1 = 2;
const uint8_t encoder2 = 4;
const uint8_t encoder3 = 7;

const uint8_t motor1Ports[] = {3, 5};
const uint8_t motor2Ports[] = {6, 9};
const uint8_t motor3Ports[] = {10, 11};

const uint8_t posSensor1 = A0;
const uint8_t posSensor2 = A1;
const uint8_t posSensor3 = A2;

const uint8_t jackpotLED = 13;
const uint8_t lockLED1 = A3;
const uint8_t lockLED2 = A6;
const uint8_t lockLED3 = A7;

// Other constants

const uint16_t initialPause = 1000;			// Initial pause before reducing speed
const uint16_t reduceStep = 30;				// Steps between each reduction
const uint16_t finalSteps = 260;			// Steps after sensor is triggered
const uint8_t maxSpeed = 180;				// Maximum speed
const uint8_t minSpeed = 120;				// Minimum speed
const unsigned long debouncePeriod = 1000;	// Microseconds

#endif	// SLOTS_H
