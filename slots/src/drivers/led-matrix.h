
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#pragma once

// -------------------------------------------------------------------- Includes

#include <MD_MAX72xx.h>
#include <MD_MAX72xx_lib.h>
// #include "../fonts/font5x7_fixed_single.h"
#include "../fonts/font4x8_fixed_num.h"
#include "../fonts/font3x5_fixed_num.h"

extern MD_MAX72XX mx;

// ----------------------------------------------------------- Class declaration

class LedMatrix
{
	private:
		const uint8_t charSpacing = 1;

	public:
		void start();
		void scrollText(const char *str, bool clear = false);
		void printText(const char *str, uint8_t offset = 0, bool clear = false);
		void printUnsignedInteger(uint16_t value, uint8_t bufferSize, uint8_t offset = 0,
			bool clear = false, bool leftPadWithZeros = false);
};

// ------------------------------------------------------------------------- End
