
//
// DFRobotmyDFPlayerMini sound driver class
//

// -------------------------------------------------------------------- Includes

#include "sound.h"
#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// --------------------------------------------------------------------- Defines

#define DEFAULT_VOLUME		18			// 0-30
#define DFPLAYER_BAUDRATE	9600

// ------------------------------------------------------------ Global variables

DFRobotDFPlayerMini myDFPlayer;

// ----------------------------------------------------- Public member functions

// https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299
// https://github.com/DFRobot/DFRobotDFPlayerMini

bool Sound::Setup(uint8_t volume = 255)
{
	Serial2.begin(DFPLAYER_BAUDRATE);
	if(!myDFPlayer.begin(Serial2)) {
		return false;
	}

	myDFPlayer.volume(volume != 255 ? volume : DEFAULT_VOLUME);
	return true;
}

void Sound::Play(uint8_t soundIndex, uint8_t volume = 255)
{
	if(volume != 255) {
		myDFPlayer.volume(volume);
	}
	myDFPlayer.play(soundIndex);
}

void Sound::Stop()
{
	myDFPlayer.stop();
}

// ------------------------------------------------------------------------- End
