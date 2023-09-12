
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include <arduino-timer.h>
#include "display.h"
#include "slots.h"

// --------------------------------------------------------------------- Defines

#define CLEAR_COL		2		// Last column to be cleared

// ------------------------------------------------------------ Global variables

auto displayBlinkTimer = timer_create_default();
bool displayBlinkState = LOW;

// ------------------------------------------------------------ Global functions

bool blinkDisplayCallback(void *)
{
	mx.control(MD_MAX72XX::SHUTDOWN, displayBlinkState);
	displayBlinkState = !displayBlinkState;
	return true;
}

// ----------------------------------------------------- Public member functions

void Display::setup()
{
	mx.control(MD_MAX72XX::INTENSITY, MX_BRIGHTNESS);
	mx.clear();
	mx.setFont(_font8_var_slots);
}

void Display::loop()
{
	displayBlinkTimer.tick();
}

void Display::showBet(uint16_t value)
{
	ledMatrix.printUnsignedInteger(value, 1, 0, NumberPadding::NONE, 16);
}

void Display::scroll(const char* msg)
{
	mx.clear(0, CLEAR_COL);
	ledMatrix.scrollText(msg, MX_TEXTPOS);
}

void Display::scrollAll(const char* msg)
{
	mx.clear();
	ledMatrix.scrollText(msg);
}

void Display::show(const char* msg)
{
	mx.clear(0, CLEAR_COL);
	ledMatrix.printText(msg, MX_TEXTPOS);
}

void Display::showAll(const char* msg, uint8_t pos = 0)
{
	mx.clear();
	ledMatrix.printText(msg, pos);
}

void Display::showCentered(const char* msg)
{
	uint8_t charSpacing = CHARSPACING;
	uint8_t charWidth;
	uint8_t length = 0;
	uint8_t strLen = strlen(msg);

	for(uint8_t i = 0; msg[i] != '\0', i < strLen; i++) {
		charWidth = mx.setChar(length, msg[i]);
		length += (charWidth + charSpacing);
	}

	mx.clear();
	ledMatrix.printText(msg, (8 * MATRIXMODULELEN - length) / 2 + charSpacing);
}

void Display::blink(bool blink, uint32_t interval = 300)
{
	if(blink) {
		displayBlinkState = HIGH;
		displayBlinkTimer.every(interval, blinkDisplayCallback);
	} else {
		displayBlinkTimer.cancel();
		mx.control(MD_MAX72XX::SHUTDOWN, LOW);
	}
}

/**
 * Display a number starting at column TEXTPOS.
 */
void Display::show(uint16_t value, bool clear)
{
	if(clear) {
		ledMatrix.clearColumns(MX_TEXTPOS, MX_NUMPOS);
	}
	ledMatrix.printUnsignedInteger(value, MAXCOINDIGITS,
		MX_NUMPOS, NumberPadding::SPACES);
}

void Display::clear()
{
	scroll("     ");
}

void Display::clearAll()
{
	scrollAll("        ");
}

// ------------------------------------------------------------------------- End
