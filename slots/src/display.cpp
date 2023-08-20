
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

void Display::welcome()
{
	mx.setFont(_sysfont);
	ledMatrix.printText("Hello", 13);
}

// TODO: show icons instead
void Display::displayBet(uint8_t value)
{
	mx.setFont(_LCD4x8_fixed_num);
	ledMatrix.printUnsignedInteger(value, 1);
}

// ------------------------------------------------------------------------- End
