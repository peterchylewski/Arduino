#include <Wire.h>
#include <OzOLED.h>

// http://blog.oscarliang.net/arduino-oled-display-library/

void setup(){

  OzOled.init();  //initialze Oscar OLED display
  OzOled.printString("Hello World!"); //Print the String

}

void loop(){
  
}

