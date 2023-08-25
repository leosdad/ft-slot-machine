
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "display.h"

// ----------------------------------------------------- Public member functions

void Display::start()
{
	mx.control(MD_MAX72XX::INTENSITY, 0);
	mx.clear();
}

void Display::show(const char* msg)
{
	mx.setFont(_sysfont);
	ledMatrix.printText(msg, 8);
}

void Display::clear()
{
	mx.setFont(_sysfont);
	ledMatrix.printText("        ", 8);
}

// TODO: show icons instead
void Display::displayBet(uint8_t value)
{
	mx.setFont(_font4x8_fixed_num);
	ledMatrix.printUnsignedInteger(value, 1);
}

// ------------------------------------------------------------------------- End
