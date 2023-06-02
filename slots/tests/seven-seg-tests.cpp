
#include <Wire.h>
#include <SevenSegDisplay.h>

void sevenSegTest()
{
	Wire.begin();

	Display::Init();
	Display::Clear();
	Display::Stop();
	Display::Show("123Ab ");
	Display::Rotate(500);
}
