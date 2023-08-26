/**
 * Single motor driver.
 */

#pragma once

// -------------------------------------------------------------------- Includes

#include <Arduino.h>

// ----------------------------------------------------------- Class declaration

/**
 * Driver for one motor using DRV8833 or equivalent module.
 */
class MotorDriver
{
	private:

		uint8_t motorOutA, motorOutB;
		uint8_t motorHome, motorEnd;
		bool reversed = false;

	public:

		// MotorDriver(const uint8_t motorPorts[2], bool reversePolarity = false);
		// MotorDriver(const uint8_t motorPort1, const uint8_t motorPort2,
		// 	bool reversePolarity = false);
		void Init(const uint8_t motorPorts[2], bool reversePolarity = false);
		void Init(const uint8_t motorPort1, const uint8_t motorPort2,
			bool reversePolarity = false);

		void FullRotateCCW();
		void FullRotateCW();
		void RotateCCW(const uint8_t speed);
		void RotateCW(const uint8_t speed);
		void Coast();
		void Brake();
};

// ------------------------------------------------------------------------- End
