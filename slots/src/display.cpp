
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

void Display::displayBet(uint8_t value)
{
	ledMatrix.printUnsignedInteger(value, 1, 0, NumberPadding::NONE, 16);
}

void Display::show(const char* msg)
{
	mx.clear(0, 2);
	ledMatrix.scrollText(msg, MX_TEXTPOS);
}

/**
 * Display a number starting at column TEXTPOS.
 */
void Display::show(uint8_t value)
{
	ledMatrix.clearColumns(MX_TEXTPOS, MX_NUMPOS);
	ledMatrix.printUnsignedInteger(value, MAXCOINDIGITS,
		MX_NUMPOS, NumberPadding::SPACES);
}

void Display::clear()
{
	show("     ");
}

// ------------------------------------------------------------------------- End
