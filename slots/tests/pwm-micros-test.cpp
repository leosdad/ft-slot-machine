
// Simple LED Brightness Control using micros()

#include <Arduino.h>
#include "src/pwm-micros.h"

PwmMicros p1, p2, p3;

void setup()
{
	p1.Setup(34, 255);
	p2.Setup(36, 128);
	p3.Setup(38, 32);
}

void loop()
{
	p1.Loop();
	p2.Loop();
	p3.Loop();
}
