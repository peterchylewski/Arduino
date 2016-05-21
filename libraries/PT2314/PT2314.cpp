#include "PT2314.h"


void PT2314::setSource(byte source) {

	source = constrain(source, 0, 3);

	write(COMMAND_AUDIO_SWITCH, source);    
}



void PT2314::setVolume(byte vol) {

	vol = constrain(vol, 0, 0x3F);

	write(COMMAND_VOL_CTRL, 0x3F - vol);
}



void PT2314::setBass(int intBass) {
	
	byte bytBass;

	intBass = constrain(intBass, -7, 7);
	
	if (intBass < 0) bytBass = 7 - abs(intBass);
	else bytBass = 15 - intBass;
	  
	write(COMMAND_BASS_CONTROL, bytBass);
}



void PT2314::setTreble(int intTreble) {
	
	byte bytTreble;

	intTreble = constrain(intTreble, -7, 7);
	
	if (intTreble < 0) bytTreble = 7 - abs(intTreble);
	else bytTreble = 15 - intTreble;

	write(COMMAND_TREBLE_CONTROL, bytTreble);
}



void PT2314::setBalance(int intBalance) {
	
	intBalance = constrain(intBalance, -31, 31);
	
	if (intBalance == 0) {
		write(COMMAND_L_ATTENUATOR, 0x00);
		write(COMMAND_R_ATTENUATOR, 0x00);
	} else {
		if (intBalance < 0) {
			write(COMMAND_L_ATTENUATOR, 0x00);
			write(COMMAND_R_ATTENUATOR, (byte)abs(intBalance));
		} else {
			write(COMMAND_L_ATTENUATOR, (byte)abs(intBalance));
			write(COMMAND_R_ATTENUATOR, 0x00);
		}
	}
}


void PT2314::write(byte command, byte val) {
	
	Wire.beginTransmission(PT2314_ADDR);
	Wire.write(command | val);
	Wire.endTransmission();
	
	delay(10);
}

int PT2314::writeI2CChar(unsigned char c) {
    Wire.beginTransmission(PT2314_ADDR); 
    Wire.write(c);   
    int rtnCode = Wire.endTransmission();
    return rtnCode;
} 