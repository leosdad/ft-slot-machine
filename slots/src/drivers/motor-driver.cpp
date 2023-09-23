/**
 * Motor driver for H-bridge modules like the DRV8833.
 * Rubem Pechansky
 */

#include "motor-driver.h"

// ----------------------------------------------------- Public member functions

/**
 * Initialize the motor.
 * @param motorPorts An array with the two motor pin numbers.
 * @param reversePolarity Reverse the motor polarity.
 */
void MotorDriver::Init(const uint8_t motorPorts[2], bool reversePolarity = false)
{
	Init(motorPorts[0], motorPorts[1], reversePolarity);
}

/**
 * Initialize the motor.
 * @param motorPort1 First motor pin number.
 * @param motorPort2 Second motor pin number.
 * @param reversePolarity Reverse the motor polarity.
 */
void MotorDriver::Init(const uint8_t motorPort1, const uint8_t motorPort2,
	bool reversePolarity = false)
{
	motorOutA = motorPort1;
	motorOutB = motorPort2;
	pinMode(motorOutA, OUTPUT);
	pinMode(motorOutB, OUTPUT);
	reversed = reversePolarity;	
}

/**
 * Rotate motor counterclockwise at full speed.
 */
void MotorDriver::FullRotateCCW()
{
	digitalWrite(motorOutA, LOW);
	digitalWrite(motorOutB, HIGH);
}

/**
 * Rotate motor clockwise at full speed.
 */
void MotorDriver::FullRotateCW()
{
	digitalWrite(motorOutA, HIGH);
	digitalWrite(motorOutB, LOW);
}

/**
 * Rotate motor counterclockwise with PWM.
 * @param speed Motor speed (0 to 255).
 */
void MotorDriver::RotateCCW(const uint8_t speed)
{
	if(!reversed) {
		analogWrite(motorOutA, 255 - speed);
		digitalWrite(motorOutB, HIGH);
	} else {
		digitalWrite(motorOutA, LOW);
		analogWrite(motorOutB, speed);
	}
}

/**
 * Rotate motor clockwise with PWM.
 * @param speed Motor speed (0 to 255).
 */
void MotorDriver::RotateCW(const uint8_t speed)
{
	if(!reversed) {
		digitalWrite(motorOutA, HIGH);
		analogWrite(motorOutB, 255 - speed);
	} else {
		analogWrite(motorOutA, speed);
		digitalWrite(motorOutB, LOW);
	}
}

/**
 * Let motor deacelerate naturally until it stops.
 */
void MotorDriver::Coast()
{
	digitalWrite(motorOutA, LOW);
	digitalWrite(motorOutB, LOW);
}

/**
 * Forces motor to stop quickly.
 */
void MotorDriver::Brake()
{
	digitalWrite(motorOutA, HIGH);
	digitalWrite(motorOutB, HIGH);
}

// ------------------------------------------------------------------------- End
