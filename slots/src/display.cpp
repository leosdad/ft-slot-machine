
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "display.h"
#include "slots.h"

// ----------------------------------------------------- Public member functions

// Remove from here
#define DIGITS 		4	// Max = 9999
#define TEXTPOS		12	// Was ((4 * 8) - (4 * (DIGITS + 1)) + 1)

void Display::start()
{
	mx.control(MD_MAX72XX::INTENSITY, MX_BRIGHTNESS);
	mx.clear();
	mx.setFont(_font8_var_slots);
}

void Display::displayBet(uint8_t value)
{
	ledMatrix.printUnsignedInteger(value, 1, 0, NumberPadding::NONE, '!');
}

void Display::show(const char* msg)
{
	mx.clear(0, 2);
	ledMatrix.scrollText(msg, TEXTPOS);
}

/**
 * Display a number starting at column TEXTPOS.
 */
void Display::show(uint8_t value)
{
	ledMatrix.clearColumns(8, 11);
	ledMatrix.printUnsignedInteger(value, DIGITS, TEXTPOS, NumberPadding::SPACES);
}

void Display::clear()
{
	show("     ");
}

// ------------------------------------------------------------------------- End
