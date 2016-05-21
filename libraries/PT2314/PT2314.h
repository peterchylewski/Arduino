/*
+----------+
| PT2314.h |
+----------+

Required the Wire.h library to be included in your Arduino Sketch.


By Andrew Wyatt
retrojdm.com

Started:		2013-10-21
Updated:		2013-10-21

Last Change:	Created this library :)
*/

#ifndef PT2314_h
#define PT2314_h

#include "Arduino.h"
#include "Wire.h"


// +----------+
// | #Defines |
// +----------+
// PT2314 I2C 4-Input Stereo Audio Processor
// Used for switching sources, volume, bass, and treble
#define PT2314_ADDR            0x44

#define CHANNEL_0              0x00
#define CHANNEL_1              0x01
#define CHANNEL_2              0x02
#define CHANNEL_3              0x03

#define COMMAND_VOL_CTRL       0x00 // Master Volume (0..63, 63 = Mute)
#define COMMAND_L_ATTENUATOR   0xC0 // L attenu      (0..31)
#define COMMAND_R_ATTENUATOR   0xE0 // R attenu      (0..31)
#define COMMAND_AUDIO_SWITCH   0x5C // Audio switch  (0..3)
#define COMMAND_BASS_CONTROL   0x60 // Bass          (0..31, None = 15)
#define COMMAND_TREBLE_CONTROL 0x70 // Treble        (0..31, None = 15)

class PT2314 {
	
public:
	void	setSource	(byte source);		// 0..3
	void	setVolume	(byte vol);			// 0..63
	void	setBass		(int intBass);		// -7..7
	void	setTreble	(int intTreble);	// -7..7
	void	setBalance	(int intBalance);	// -31..31
	int		writeI2CChar	(unsigned char);	

private:
	void	write		(byte command, byte val);
};

#endif