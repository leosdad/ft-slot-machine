
#include <Arduino.h>

#include "../../slots.h"
#include "../oled-display.h"

OledDisplay odt;

#define INTERVAL 500

void oledDisplayTestsSetup()
{
	odt.Setup(false);
	odt.DisplayBet(9);
}

void oledDisplayTestsLoop()
{
	odt.DisplayPayoff(9);
	delay(1000);

	odt.DisplayPayoff(80);
	delay(1000);

	odt.DisplayPayoff(710);
	delay(1000);

	odt.DisplayPayoff(8654);
	delay(1000);

	odt.DisplayPayoff(123);
	delay(1000);

	odt.DisplayPayoff(65);
	delay(1000);
}
