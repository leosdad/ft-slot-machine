
#include "./oled-display-driver.h"

// -------------------------------------------------------------------- Includes

#define TINY4KOLED_QUICK_BEGIN
#include <Tiny4kOLED.h>

#include <font8x16atari.h>
#include <font11x16.h>
#include <font16x32digits.h>
#include <ModernDos.h>

// ----------------------------------------------------- Public member functions

void OledDisplayDriver::Setup()
{
	oled.begin(128, 64, sizeof(tiny4koled_init_128x64br), tiny4koled_init_128x64br);
	oled.enableChargePump(); // The default is off, but most boards need this.
    oled.setRotation(1);     // The default orientation is not the most commonly used.
	oled.clear();
	oled.on();

	oled.setFont(FONT8X16);	// Set default font
}

void OledDisplayDriver::SetFont(Font font)
{
	switch(font) {

		default:
		case Font::MONO_SMALL:
			// Standard font, 8 x 16, monospaced
			oled.setFont(FONT8X16);
			break;

		case Font::MONO_ALTER:
			// MS-DOS font, 8 x 16, monospaced
			oled.setFont(FONT8X16MDOS);
			break;

		case Font::MONO_BOLD:
			// Atari font, 8 x 16, bold, higher than FONT8X16
			oled.setFont(FONT8X16ATARI);
			break;

		case Font::SMALL:
			// Standard font, 8 x 16, proportional
			oled.setFont(FONT8X16P);
			break;

		case Font::DIGITS_LARGE:
			// Large, 11 x 16, with digits and symbols
			oled.setFont(FONT11X16);
			break;

		case Font::DIGITS_EXTRALARGE:
			// Very large, 16 x 32, monospaced, digits only
			oled.setFont(FONT16X32DIGITS);
			break;
	}
}

void OledDisplayDriver::PrintS(uint8_t row, uint8_t col, const char* str)
{
	switch(row) {
		case 2:
		case 3:
			oled.switchRenderFrame();
			break;
	}

	switch(row) {
		case 0:
		case 2:
			oled.setCursor(col * 8, 0);
			break;
		case 1:
		case 3:
			oled.setCursor(col * 8, 2);
			break;
	}

	oled.print(str);

	switch(row) {
		case 2:
		case 3:
			oled.switchRenderFrame();
			break;
	}
}

void OledDisplayDriver::PrintN(uint8_t row, uint8_t col, uint16_t number)
{
	char buffer[16];
	ultoa(number, buffer, 10);
	PrintS(row, col, buffer);
}

// ------------------------------------------------------------------------- End