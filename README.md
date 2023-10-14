
# Slot machine game with fischertechnik and Arduino Mega

This is a very complex project I designed, built and developed for many months in 2023. This is the code that has made its way into the actual machine. This project is heavily inspired by [Hypervisual Century](http://hypervisual.com/century/), a software slot machine game that I developed as a demo for my company back in 1998.

## Mechanics and gameplay

![Close-up of the reels (uncovered)](docs/reels.jpg)

The slot machine is assembled with [fischertechnik](https://www.fischertechnik.de/en) parts. It has three reels and one payline. A very special feature is the ability to lock one or two reels upon certain conditions. This is combined with the ability to adjust the bet value, allowing some strategic decisions to achieve the goal which is to win 800 points in up to 50 spins. Despite the retro look, the entire operation is fully software-based.

## Controller

The main controller is an Arduino Mega with two custom shields, one on top of the other. It is explained in more detail in the [fischertechnik community forum](https://forum.ftcommunity.de/viewtopic.php?f=8&t=8018). Additional information is available in the [docs](/docs) folder.

![Controller](docs/controller.jpg)

## Related links

- [Best hardware approach for complex project?](https://forum.ftcommunity.de/viewtopic.php?f=8&t=7890) in the fischertechnik community
- [Arduino Mega Controller for fischertechnik](https://forum.ftcommunity.de/viewtopic.php?f=8&t=8018) in the fischertechnik community
- [Hypervisual Century for Windows](http://hypervisual.com/century/files/century_en.zip) (still playable in glorious 640 × 480 pixels)

## Third-party libraries and tools

- [arduino-timer](https://github.com/contrem/arduino-timer) by Michael Contreras
- [DFPlayerMini_Fast](https://github.com/PowerBroker2/DFPlayerMini_Fast) by PowerBroker2
- [DriveSort](http://www.anerty.net/software/file/DriveSort/) FAT file sorter by anerty
- [ezButton & ezLED](https://github.com/ArduinoGetStarted) by ArduinoGetStarted
- [FreeSound](https://freesound.org/) audio database
- [MD_MAX72xx](https://github.com/MajicDesigns/MD_MAX72XX) by Marco Colli
- [TrueRandom](https://github.com/sirleech/TrueRandom) by sirleech

## Extras

- There are various reference images, diagrams and spreadsheets in the [docs](/docs) folder.
- Includes a specially designed compact font for the MD_MAX72xx library in the [fonts](https://github.com/leosdad/ft-slot-machine/tree/master/slots/src/drivers) folder.
