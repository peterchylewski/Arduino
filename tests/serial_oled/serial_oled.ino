#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

String inputString = "";
boolean stringComplete = false;

int incomingByte = 0;   // for incoming serial data

void setup() {

  Serial.begin(9600);
  Serial.print("Start");

  inputString.reserve(200);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();   // clears the screen and buffer
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("FOO");
  display.display();     // show splashscreen
}

void loop() {

  
  if (Serial.available() > 0) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
      Serial.println(inputString);
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print(inputString);
      display.display();

      inputString = "";
      stringComplete = false;

      Serial.println("OK");
    }
  }
}


