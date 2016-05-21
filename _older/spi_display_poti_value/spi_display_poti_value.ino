 /*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x32 size display using SPI to communicate
4 or 5 pins are required to interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// If using software SPI (the default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// ---------------------- set up analog sensors

// set up smoothing of readings
const int numReadings = 100;

// set up pot
int potPin = 2;
int potValue;

int potReadings[numReadings];      // the readings from the analog input
int potIndex = 0;                  // the index of the current reading
int potTotal = 0;                  // the running total
int potAverage = 0;                // the average

// set up photo cell (https://learn.adafruit.com/photocells/using-a-photocell)
int photocellPin = 0;     // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the sensor divider

int photoCellReadings[numReadings];      // the readings from the analog input
int photoCellIndex = 0;                  // the index of the current reading
int photoCellTotal = 0;                  // the running total
int photoCellAverage = 0;                // the average

float barRatio = 128.0f / 1024.0f;
int barWidth = 0;

// set up push button
int pushButtonPin = 7;                   // choose the input pin (for a pushbutton)
int lastPushButtonVal = 0;
int pushButtonVal = 0;

// set up serial connection stuff
String inputString = "";         // a string to hold incoming data
String displayInputString = ""; 
boolean stringComplete = false;  // whether the string is complete

// set up beeper pin
int beeperPin = 8; 

// set string buffer for serial output
char buffer[50];

void setup()   {
    
  // --- initialize serial connection
  
  // start serial:
  Serial.begin(9600);
  
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  
  
  // --- initialize display
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // init done
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  //display.display();
  //delay(1000);

  display.clearDisplay();   // Clear the buffer.
  display.setTextSize(4);
  display.setTextColor(WHITE);
  //display.invertDisplay(true);
  
   // --- initialize readings arrays
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    potReadings[thisReading] = 0;
    photoCellReadings[thisReading] = 0;
  }
  
  // --- initialize button
  pinMode(pushButtonPin, INPUT);    // declare pushbutton as input
  
  // --- initialize beeper
  //pinMode(beeperPin, OUTPUT);    
  //digitalWrite(beeperPin, LOW);   

}

void loop() {
  
 //Mouse.move(0, potValue, 0);
 // Keyboard.println(potValue);
  
  display.clearDisplay();
  
  // --- display averaged pot value
  potValue = analogRead(potPin);
  potTotal= potTotal - potReadings[potIndex];     // subtract the last reading      
  potReadings[potIndex] = potValue;               // read from the sensor
  potTotal = potTotal + potReadings[potIndex];    // add the reading to the total
  potIndex = potIndex + 1;                        // advance to the next position in the array  
    if (potIndex >= numReadings)                  // if we're at the end of the array...  
      potIndex = 0;                               // ...wrap around to the beginning                 
    potAverage = potTotal / numReadings;          // calculate the average
    
  display.setCursor(2, 2);
  display.setTextSize(4);
  display.println(1023 - potAverage);
  
  //Serial.print(1023 - potAverage);
   
   // display bar graph
   barWidth = int((1023 - potAverage) * barRatio);
   //Serial.println(potAverage);
   
   
   sprintf(buffer, "{\"potValue\": %d}", potAverage);
   Serial.println(buffer);
  
   display.fillRect(0, 31, barWidth, 1, 1);
   
   // --- display averaged photo cell value
  photocellReading = analogRead(photocellPin);
  photoCellTotal = photoCellTotal - photoCellReadings[photoCellIndex];    // subtract the last reading      
  photoCellReadings[photoCellIndex] = photocellReading;                           // read from the sensor
  photoCellTotal = photoCellTotal + photoCellReadings[photoCellIndex];    // add the reading to the total
  photoCellIndex = photoCellIndex + 1;                                    // advance to the next position in the array  
  if (photoCellIndex >= numReadings)                                      // if we're at the end of the array...  
      photoCellIndex = 0;                                                 // ...wrap around to the beginning                 
    photoCellAverage = photoCellTotal / numReadings;                      // calculate the average
    
  display.setCursor(78, 2);
  display.setTextSize(1);
  display.println(photoCellAverage);
  
  sprintf(buffer, "{\"photoCellValue\": %d}", photoCellAverage);
  Serial.println(buffer);
   
  // --- display button state
  
  pushButtonVal = digitalRead(pushButtonPin);     // read input value
  display.invertDisplay(pushButtonVal == HIGH);   // check if the input is HIGH (button released)
  
  if (pushButtonVal != lastPushButtonVal) {
    Serial.write((pushButtonVal == HIGH) ? "down" : "up");    
    lastPushButtonVal = pushButtonVal;
  }
  
  display.setCursor(78, 12);
  display.setTextSize(1);
  display.println(pushButtonVal);
  
  // --- listen to serial port
  
  serialEvent();
  if (stringComplete) {
    displayInputString = inputString;
    //Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  
  if (displayInputString != "") {
    display.setCursor(78, 23);
    display.setTextSize(1);
    display.println(displayInputString);
  }
  
  display.display();
  
  //delay(50);
}

// http://www.hobbytronics.co.uk/arduino-tutorial7-piezo-beep
void beep(unsigned char delayms){
  analogWrite(beeperPin, 20);      // Almost any value can be used except 0 and 255
                                   // experiment to get the best tone
  delay(delayms);              // wait for a delayms ms
  analogWrite(beeperPin, 0);   // 0 turns it off
  delay(delayms);              // wait for a delayms ms   
}  


/*
  SerialEvent occurs whenever a new data comes in the
hardware serial RX.  This routine is run between each
time loop() runs, so using delay inside loop can delay
response.  Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}
