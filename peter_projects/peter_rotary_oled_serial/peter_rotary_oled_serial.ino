/* Read Quadrature Encoder
  * Connect Encoder to Pins encoder0PinA, encoder0PinB, and +5V.
  *
  * Sketch by max wolf / www.meso.net
  * v. 0.1 - very basic functions - mw 20061220
  *
  */

int val = -1;
int encoder0PinA = 5;
int encoder0PinB = 6;
int encoder0PinClick = 4;
int encoder0Pos = 0;
int encoder0PinALast = LOW;
int n = LOW;
int c = LOW;
int clickstate = LOW;

// -------------- prepare display

#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

void setup() {

  // -------------- setup encoder

  pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);
  pinMode (encoder0PinClick, INPUT);

  // -------------- setup display

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  display.clearDisplay();

  display.setCursor(6, 0);
  display.setTextSize(8);
  display.println("*FOO*");
  display.display();

  // -------------- setup serial

    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
    
  Serial.begin(115200);

  //while(!Serial);
  
  /*
  while (!Serial) {
    ; // wait for Serial to connect
  }
  */
}

void loop() {

  c = digitalRead(encoder0PinClick);
  if (c != clickstate) {
    if (c == LOW) {
      Serial.print ("click down\n");
    } else {
      Serial.print ("click up\n");
    }
    clickstate = c;
  }

  n = digitalRead(encoder0PinA);
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encoder0PinB) == LOW) {
      encoder0Pos++;
    } else {
      encoder0Pos--;
    }
    Serial.print (encoder0Pos);
    Serial.print ("\n");

    display.clearDisplay();
    display.setCursor(6, 0);
    display.println(encoder0Pos);
    display.display();
  }
  encoder0PinALast = n;
}
