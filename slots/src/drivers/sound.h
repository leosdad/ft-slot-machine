
//
// DFRobotDFPlayerMini sound driver class
//

#pragma once

// -------------------------------------------------------------------- Includes

#include <Arduino.h>

// ----------------------------------------------------------- Class declaration

class Sound
{
	public:
		bool Setup(uint8_t volume = 255);
		void Play(uint8_t soundIndex, uint8_t volume = 255);
		void Stop();
};

// ------------------------------------------------------------------------- End
