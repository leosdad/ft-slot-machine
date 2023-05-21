
#include "oled-display.h"

// https://github.com/datacute/Tiny4kOLED

#define TINY4KOLED_QUICK_BEGIN

#include <Tiny4kOLED.h>

void oledSetup()
{
	// TODO: Works only with FONT8X16
	oled.begin(128, 64, sizeof(tiny4koled_init_128x64br), tiny4koled_init_128x64br);
	oled.setFont(FONT8X16);
	oled.clear();
	oled.on();
}

void oledPrint(uint8_t row, uint8_t col, char *str)
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

void oledClearRow(uint8_t row)
{
	oledPrint(row, 0, "                ");
}