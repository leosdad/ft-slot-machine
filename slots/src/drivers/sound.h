
//
// DFRobotDFPlayerMini sound driver class
//

#pragma once

// -------------------------------------------------------------------- Includes

#include <Arduino.h>

// --------------------------------------------------------------------- Defines

#define NULL_VOLUME			255

// ----------------------------------------------------------- Class declaration

class Sound
{
	public:
		bool Setup(uint8_t volume = NULL_VOLUME);
		void Play(uint8_t soundIndex, uint8_t volume = NULL_VOLUME);
		void Stop();
		void Reset();
};

// ------------------------------------------------------------------------- End
