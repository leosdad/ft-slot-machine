
// Time division PWM driver with micros()
// With the help of ChatGPT

#pragma once

// -------------------------------------------------------------------- Includes

#include <Arduino.h>

// --------------------------------------------------------------------- Defines

#define DEFAULTCYCLEDURATION 10000  // 10 ms in μs

// ----------------------------------------------------------- Class declaration

/**
 * Time division PWM driver with micros().
 */
class PwmMicros
{
	private:
		uint8_t hardwarePin;
		unsigned long pwmOnTime;
		unsigned long pwmOffTime;
		unsigned long previousMicros;
		unsigned long pwmCycleDuration;
		bool ledState;

	public:
		void Setup(const uint8_t pin, const uint8_t pwmValue, const uint32_t cycleDuration = DEFAULTCYCLEDURATION);
		void SetValue(const uint8_t value);
		void Set(bool on);
		void TurnOn();
		void TurnOff();
		void Loop();
};

// ------------------------------------------------------------------------- End
