
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#pragma once

// -------------------------------------------------------------------- Includes

#include <MD_MAX72xx.h>

#include "../fonts/font8_var_slots.h"

// ----------------------------------------------------------------------- Enums

enum class NumberPadding {
	NONE = 0,
	SPACES,
	ZEROS,
};

// ------------------------------------------------------------------- Variables

extern MD_MAX72XX mx;

// ----------------------------------------------------------- Class declaration

class LedMatrix
{
	private:

		const uint8_t charSpacing = 1;
		uint8_t columnZero;

	public:

		void start();
		void clearColumns(uint8_t start, uint8_t end);
		void scrollText(const char *str, uint8_t offset = 0);
		void printText(const char *str, uint8_t offset = 0);
		void printUnsignedInteger(
			uint16_t value,
			uint8_t bufferSize,
			uint8_t offset = 0,
			NumberPadding padding = NumberPadding::NONE,
			uint8_t zero = '0'
		);
};

// ------------------------------------------------------------------------- End
