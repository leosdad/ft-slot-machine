
#include <Arduino.h>
#include <DFPlayerMini_Fast.h>
#include "oled-display.h"

DFPlayerMini_Fast mp3Player;

void dfPlayerTestsSetup()
{
	Serial2.begin(9600);
	mp3Player.begin(Serial2, true);

	if(!mp3Player.begin(Serial2, false, 500)) {
		oledPrint(0, 0, "Unable to begin");
		while(true) {
		}
	}

	// Set volume value (From 0 to 30)
	mp3Player.volume(15);

	oledPrint(0, 0, "MP3 player tests");
}

void dfPlayerTests()
{
	oledPrint(1, 0, "Track 1");
	mp3Player.play(1);
	delay(2000);

	oledPrint(1, 0, "Track 2");
	mp3Player.play(2);
	delay(2000);

	oledPrint(1, 0, "Track 3");
	mp3Player.play(3);
	delay(2000);
}