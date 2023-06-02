
#include <Arduino.h>
#include <Servo.h>
#include "../oled-display.h"

Servo _servo1;
int _servo_pos = 0;

void servoTestsSetup()
{
	_servo1.attach(44);
}

void servoTests()
{
	for(_servo_pos = 180; _servo_pos >= 0; _servo_pos -= 1) {
		_servo1.write(_servo_pos);
		oledPrint(2, _servo_pos / 13, " | ");
		// delay(10);
	}
	delay(500);

	for(_servo_pos = 0; _servo_pos <= 180; _servo_pos += 1) {
		_servo1.write(_servo_pos);
		oledPrint(2, _servo_pos / 13, " | ");
		// delay(10);
	}
	delay(500);
}