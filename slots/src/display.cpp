
// fischertechnik / Arduino Slots
// Rubem Pechansky 2023

// -------------------------------------------------------------------- Includes

#include <arduino-timer.h>
#include "display.h"
#include "slots.h"

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
	mx.clear(0, 2);
	ledMatrix.scrollText(msg, MX_TEXTPOS);
}

void Display::scrollAll(const char* msg)
{
	mx.clear();
	ledMatrix.scrollText(msg);
}

void Display::show(const char* msg)
{
	mx.clear(0, 2);
	ledMatrix.printText(msg, MX_TEXTPOS);
}

void Display::showAll(const char* msg)
{
	mx.clear();
	ledMatrix.printText(msg, 0);
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

// ------------------------------------------------------------------------- End
