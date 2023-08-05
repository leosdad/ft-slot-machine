
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "seven-seg.h"

// ----------------------------------------------------- Public member functions

// Reset seven-segment display
void SevenSeg::Setup()
{
	Wire.begin();
	Display::Init(false);
}

void SevenSeg::DisplayNumber(unsigned long value)
{
	Display::Stop();
	Display::U2s(displayBuffer, value);
	Display::Show(displayBuffer);
}

void SevenSeg::ScrollMessage(char* msg)
{
	Display::Show(msg);
	Display::Rotate(100);
}

void SevenSeg::FlashMessage(char* msg)
{
	Display::Show(msg);
	Display::Flash(200);
}

// ------------------------------------------------------------------------- End
