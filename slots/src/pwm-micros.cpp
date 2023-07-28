
// PWM with micros()
// This implementation supports PWM with values ranging from 0 to 255
// 0 means fully off, 255 means fully on

#include "pwm-micros.h"

/**
 * Setup. Default is 10 ms cycle.
 */
void PwmMicros::Setup(const uint8_t pin, uint32_t cycleDuration = DEFAULTCYCLEDURATION)
{
	pwmCycleDuration = cycleDuration;
	hardwarePin = pin;
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
}

void PwmMicros::Setup(const uint8_t pin, const uint8_t value)
{
	Setup(pin);
	SetValue(value);
}

void PwmMicros::Loop()
{
	unsigned long currentMicros = micros();

	if(pwmOnTime > 0) {
		if(currentMicros - previousMicros >= (ledState ? pwmOnTime : pwmOffTime)) {
			ledState = !ledState;
			digitalWrite(hardwarePin, ledState);
			previousMicros = currentMicros;
		}
	} else {
		digitalWrite(hardwarePin, LOW);
	}
}

// Set the PWM value (0 to 255)
void PwmMicros::SetValue(uint8_t value)
{
	pwmOnTime = pwmCycleDuration * constrain(value, 0, 255) / 256;
	pwmOffTime = pwmCycleDuration - pwmOnTime;
}
