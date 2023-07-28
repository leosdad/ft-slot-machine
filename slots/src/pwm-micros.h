
// Time division PWM driver
// With the help of ChatGPT

#ifndef PWM_MICROS_H
#define PWM_MICROS_H

#include <Arduino.h>

#define DEFAULTCYCLEDURATION 10000  // 10 ms in microseconds

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
		void Setup(const uint8_t pin, uint32_t cycleDuration = DEFAULTCYCLEDURATION);
		void Setup(const uint8_t pin, const uint8_t value);
		void SetValue(const uint8_t value);
		void Loop();
};

#endif // PWM_MICROS_H
