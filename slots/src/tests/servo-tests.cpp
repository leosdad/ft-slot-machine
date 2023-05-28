
#include <Arduino.h>
#include <Servo.h>
#include "../oled-display.h"

Servo servo1;

int pos = 0;

void servoTestsSetup()
{
	servo1.attach(44);
}

void servoTests()
{
	for(pos = 180; pos >= 0; pos -= 1) {
		servo1.write(pos);
		oledPrint(2, pos / 13, " | ");
		// delay(10);
	}
	delay(500);

	for(pos = 0; pos <= 180; pos += 1) {
		servo1.write(pos);
		oledPrint(2, pos / 13, " | ");
		// delay(10);
	}
	delay(500);
}