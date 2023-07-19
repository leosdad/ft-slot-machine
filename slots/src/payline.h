
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

#ifndef PAYLINE_H
#define PAYLINE_H

// -------------------------------------------------------------------- Includes

#include "../slots.h"

// ----------------------------------------------------------- Class declaration

class Payline
{
	public:
		uint8_t GetLineSymbol(uint8_t line, uint8_t reel, uint16_t pos[]);
};

// ------------------------------------------------------------------------- End

#endif // PAYLINE_H
