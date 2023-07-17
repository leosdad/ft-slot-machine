
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

// ----------------------------------------------------------------------- Types

enum class Font {
	MONO_SMALL = 0,
	MONO_ALTER,
	MONO_BOLD,
	SMALL,
	DIGITS_LARGE,
	DIGITS_EXTRALARGE,
};

// -------------------------------------------------------------------- Includes

#include <Arduino.h>

// ----------------------------------------------------------- Class declaration

class OledDisplay
{
	public:
		void Setup();
		void PrintS(uint8_t row, uint8_t col, const char *str);
		void PrintN(uint8_t row, uint8_t col, uint16_t number);
		void SetFont(Font font);
};

// ------------------------------------------------------------------------- End

#endif // OLED_DISPLAY_H
