/**
 * Motor driver for H-bridge modules like the DRV8833.
 * Rubem Pechansky
 */

#include "motor-driver.h"

// ------------------------------------------------------------- Private methods

void MotorDriver::initMotor(const uint8_t motorPorts[2])
{
	motorOutA = motorPorts[0];
	motorOutB = motorPorts[1];
	pinMode(motorOutA, OUTPUT);
	pinMode(motorOutB, OUTPUT);
}

void MotorDriver::initEncoder(const uint8_t _encoder)
{
	encoder = _encoder;
	pinMode(encoder, INPUT_PULLUP);
}

// void MotorDriver::initSwitches(const uint8_t switches[2], bool pullups)
// {
// 	motorHome = switches[0];
// 	motorEnd = switches[1];
// 	if(pullups) {
// 		pinMode(motorHome, INPUT_PULLUP);
// 		pinMode(motorEnd, INPUT_PULLUP);
// 	}
// }

// ---------------------------------------------------------------- Constructors

// MotorDriver::MotorDriver(const uint8_t motorPorts[2], const uint8_t encoder, const uint8_t switches[2], bool pullups)
// {
// 	initMotor(motorPorts);
// 	initEncoder(encoder);
// 	initSwitches(switches, pullups);
// }

MotorDriver::MotorDriver(const uint8_t motorPorts[2], const uint8_t encoder)
{
	initMotor(motorPorts);
	initEncoder(encoder);
}

MotorDriver::MotorDriver(const uint8_t motorPorts[2])
{
	initMotor(motorPorts);
}

MotorDriver::MotorDriver()
{
}

// MotorDriver::MotorDriver(const uint8_t motorPortA, const uint8_t motorPortB)
// {
// 	const uint8_t v[2] = {motorPortA, motorPortB};
// 	initMotor(v);
// }

// -------------------------------------------------------------- Public methods

void MotorDriver::Init(const uint8_t motorPorts[2])
{
	initMotor(motorPorts);
}

void MotorDriver::Init(const uint8_t motorPorts[2], const uint8_t encoder)
{
	initMotor(motorPorts);
	initEncoder(encoder);
}

// Rotate at full speed

void MotorDriver::FullRotateCCW()
{
	digitalWrite(motorOutA, HIGH);
	digitalWrite(motorOutB, LOW);
}

void MotorDriver::FullRotateCW()
{
	digitalWrite(motorOutA, LOW);
	digitalWrite(motorOutB, HIGH);
}

// Rotate with PWM

void MotorDriver::RotateCCW(const uint8_t speed)
{
	if(reversePolarity) {
		digitalWrite(motorOutA, HIGH);
		analogWrite(motorOutB, 255 - max(0, min(255, speed)));
	} else {
		digitalWrite(motorOutA, LOW);
		analogWrite(motorOutB, max(0, min(255, speed)));
	}
}

void MotorDriver::RotateCW(const uint8_t speed)
{
	if(reversePolarity) {
		analogWrite(motorOutA, 255 - max(0, min(255, speed)));
		digitalWrite(motorOutB, HIGH);
	} else {
		analogWrite(motorOutA, max(0, min(255, speed)));
		digitalWrite(motorOutB, LOW);
	}
}

void MotorDriver::Coast()
{
	digitalWrite(motorOutA, LOW);
	digitalWrite(motorOutB, LOW);
}

void MotorDriver::Brake()
{
	digitalWrite(motorOutA, HIGH);
	digitalWrite(motorOutB, HIGH);
}

void MotorDriver::SetPolarity(bool negative = false)
{
	reversePolarity = negative;
}
