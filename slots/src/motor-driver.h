/**
 * Single motor driver
 */

#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Arduino.h>

/**
 * Driver for one motor using DRV8833 module.
 */
class MotorDriver
{
	private:

		// ------------------------ Private types

		// Motor states

		enum {
			COAST = 0,
			FORWARD,
			BACKWARDS,
			HOME,
			CCW,
			CW,
			GOINGHOME,
			BRAKE,
			RESET,
		};

		// ------------------------ Private fields

		// Arduino pins

		uint8_t motorOutA, motorOutB;
		uint8_t encoder;
		uint8_t motorHome, motorEnd;
		bool reversePolarity = false;

		// ------------------------ Private methods

		void initMotor(const uint8_t motorPorts[2]);
		void initEncoder(const uint8_t encoder);
		void initSwitches(const uint8_t switches[2], bool pullups);

	public:

		/**
		 * Constructor.
		*/
		// MotorDriver(const uint8_t motorPorts[2], const uint8_t encoder, const uint8_t switches[2], bool pullups);
		MotorDriver(const uint8_t motorPorts[2], const uint8_t encoder);
		// MotorDriver(const uint8_t motorPortA, const uint8_t motorPortB);
		MotorDriver(const uint8_t motorPorts[2]);
		MotorDriver();

		void Init(const uint8_t motorPorts[2]);
		void Init(const uint8_t motorPorts[2], const uint8_t encoder);

		/**
		 * Rotate motor counterclockwise.
		*/
		void FullRotateCCW();
		void RotateCCW(const uint8_t speed);

		/**
		 * Rotate motor clockwise.
		*/
		void FullRotateCW();
		void RotateCW(const uint8_t speed);

		/**
		 * Let motor deacelerate naturally.
		*/
		void Coast();

		/**
		 * Forces motor to stop quickly.
		*/
		void Brake();

		/**
		 * Sets the driver polarity.
		*/
		void SetPolarity(bool reversePolarity);
};

#endif	// MOTOR_DRIVER_H

// -----------------------------------------------------------------------------
