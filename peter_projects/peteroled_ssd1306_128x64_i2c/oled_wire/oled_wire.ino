#include <Wire.h>

Wire.beginTransmission(0x78); // might be different for your display
Wire.write(0x40);
Wire.write(0x01);
Wire.endTransmission(); // stop transmitting
