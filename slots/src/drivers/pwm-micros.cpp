
// Time division PWM driver with micros()
// With the help of ChatGPT

// -------------------------------------------------------------------- Includes

#include "pwm-micros.h"

// ----------------------------------------------------- Public member functions

/**
 * Constructor.
 * 
 * @param pin PWM pin number.
 * @param pwmValue PWM value from 0 to 255. 0 means fully off, 255 is fully on.
 * @param cycleDuration Total duration of a complete PWM cycle in μs.
 */
void PwmMicros::Setup(const uint8_t pin, const uint8_t pwmValue, const uint32_t cycleDuration = DEFAULTCYCLEDURATION)
{
	pwmCycleDuration = cycleDuration;
	hardwarePin = pin;
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
	SetValue(pwmValue);
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

// Boolean set to maximum brightness or fully off.
void PwmMicros::Set(bool on)
{
	SetValue(on ? 255 : 0);
}

void PwmMicros::TurnOn()
{
	SetValue(255);
}

void PwmMicros::TurnOff()
{
	SetValue(0);
}

// ------------------------------------------------------------------------- End
