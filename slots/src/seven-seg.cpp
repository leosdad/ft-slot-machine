
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include "seven-seg.h"

// -------------------------------------------------------------- Public methods

// Reset seven-segment display
void SevenSeg::Setup()
{
	Wire.begin();
	Display::Init();
	Display::Clear();
	Display::Stop();
}

void SevenSeg::DisplayNumber(unsigned long value)
{
	Display::U2s(displayBuffer, value);
	Display::Show(displayBuffer);
}

// ------------------------------------------------------------------------- End
