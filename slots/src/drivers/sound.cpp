
//
// DFPlayerMini_Fast sound driver class
//

// -------------------------------------------------------------------- Includes

#include "sound.h"
#include <HardwareSerial.h>
#include <DFPlayerMini_Fast.h>

// ------------------------------------------------------------------ References

// https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299
// https://forum.arduino.cc/t/1019399 (how to eliminate "pop" sound)
// https://forum.arduino.cc/t/new-and-improved-dfplayer-mini-library/492914/5?page=4
// https://github.com/PowerBroker2/DFPlayerMini_Fast

// --------------------------------------------------------------------- Defines

#define PLAYER_TIMEOUT		200
#define DEFAULT_VOLUME		18			// 0-30
#define DFPLAYER_BAUDRATE	9600

// ------------------------------------------------------------ Global variables

DFPlayerMini_Fast myDFPlayer;

// ----------------------------------------------------- Public member functions

bool Sound::Setup(uint8_t volume = NULL_VOLUME)
{
	Serial2.begin(DFPLAYER_BAUDRATE);
	if(!myDFPlayer.begin(Serial2, false, PLAYER_TIMEOUT)) {
		return false;
	}

	myDFPlayer.stop();
	myDFPlayer.volume(volume != NULL_VOLUME ? volume : DEFAULT_VOLUME);
	return true;
}

void Sound::Play(uint8_t soundIndex, uint8_t volume = NULL_VOLUME)
{
	if(volume != NULL_VOLUME) {
		myDFPlayer.volume(volume);
	}
	myDFPlayer.play(soundIndex);
}

void Sound::Stop()
{
	myDFPlayer.stop();
}

void Sound::Reset()
{
	myDFPlayer.reset();
}

// ------------------------------------------------------------------------- End
