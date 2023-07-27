
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef OLED_DISPLAY_DRIVER_H
#define OLED_DISPLAY_DRIVER_H

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

class OledDisplayDriver
{
	public:
		void Setup();
		void SetFont(Font font);
		void PrintS(uint8_t row, uint8_t col, const char *str);
		void PrintN(uint8_t row, uint8_t col, uint16_t number);
};

// ------------------------------------------------------------------------- End

#endif // OLED_DISPLAY_DRIVER_H
