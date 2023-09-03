
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "display.h"
#include "slots.h"

// ----------------------------------------------------- Public member functions

void Display::setup()
{
	mx.control(MD_MAX72XX::INTENSITY, MX_BRIGHTNESS);
	mx.clear();
	mx.setFont(_font8_var_slots);
}

void Display::showBet(uint16_t value)
{
	ledMatrix.printUnsignedInteger(value, 1, 0, NumberPadding::NONE, 16);
}

void Display::scroll(const char* msg)
{
	mx.clear(0, 2);
	ledMatrix.scrollText(msg, MX_TEXTPOS);
}

void Display::scrollAll(const char* msg)
{
	mx.clear();
	ledMatrix.scrollText(msg);
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

// ------------------------------------------------------------------------- End
