/* Encoder Library - Basic Example
   http://www.pjrc.com/teensy/td_libs_Encoder.html

   This example code is in the public domain.
*/

// -------------- prepare display

#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// -------------- prepare encoder

#include <Encoder.h>

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(2, 3);
//   avoid using pins with LEDs attached

// -------------- prepare serial 

String inputString = "";
boolean stringComplete = false;


void setup() {
  Serial.begin(115200);
  inputString.reserve(200);
  
  // -------------- setup display

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  display.clearDisplay();

  display.setCursor(6, 0);
  display.setTextSize(8);
  display.println("*FOO*");
  display.display();

}

long oldPosition  = -999;

void loop() {
  
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.println(newPosition);
    display.clearDisplay();
    display.setCursor(6, 0);
    display.setTextSize(8);
    display.println(newPosition);
    display.display();
  }


  if (Serial.available() > 0) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
      //Serial.println(inputString);
      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.print(inputString);
      display.display();

      inputString = "";
      stringComplete = false;

      //Serial.println("OK");
    }
  }
}
