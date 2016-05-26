// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      5
#define NUMSTEPS       255

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int currR = 0;
int currG = 0;
int currB = 0;

int delayval = 10;

void setup() {
  //Serial.begin(9600);

  pixels.begin(); // This initializes the NeoPixel library.
  for (int i = 0; i < NUMSTEPS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
  pixels.show();
  delay(100);
}


void fadeTo(int r, int g, int b, int msec) {

  int stepR = int((currR - r) / NUMSTEPS);
  int stepG = int((currG - g) / NUMSTEPS);
  int stepB = int((currB - b) / NUMSTEPS);


  //int numSteps =

  for (int i = 0; i < NUMSTEPS; i++) {
    //Serial.write(currR);
    currR = r + stepR / i;
    currG = g + stepG / i;
    currB = b + stepB / i;
    //printf(currR);
    pixels.setPixelColor(0, pixels.Color(currR, currG, currB));
    pixels.setPixelColor(1, pixels.Color(currR, currG, currB));
    pixels.show();
    delay(delayval);
  }
  currR = r; currG = g; currB = b;
}

void loop() {

  /*
    fadeTo(0, 0, 0, 1000);
    fadeTo(255, 255, 255, 1000);
    fadeTo(255, 0, 0, 1000);
    fadeTo(0, 255, 0, 1000);
    fadeTo(0, 0, 255, 1000);
    return;
  */

  for (int i = 0; i < NUMSTEPS; i++) {
    pixels.setPixelColor(0, pixels.Color(i, 0, 0));
    pixels.setPixelColor(1, pixels.Color(0, 0, i));
    pixels.setPixelColor(2, pixels.Color(NUMSTEPS - i, i, 0));
    pixels.setPixelColor(3, pixels.Color(NUMSTEPS, i, NUMSTEPS - i));
    //pixels.setPixelColor(4, pixels.Color(0, 0, NUMSTEPS));
    // pixels.setBrightness(i);
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayval); // Delay for a period of time (in milliseconds).
  }

  delay(1000);

  for (int i = NUMSTEPS; i > 0 ; i--) {
    pixels.setPixelColor(0, pixels.Color(i, 0, 0));
    pixels.setPixelColor(1, pixels.Color(0, 0, i));
    pixels.setPixelColor(2, pixels.Color(NUMSTEPS - i, i, 0));
    pixels.setPixelColor(3, pixels.Color(NUMSTEPS, i, NUMSTEPS - i));
    //pixels.setPixelColor(4, pixels.Color(i, NUMSTEPS - i, NUMSTEPS));
    // pixels.setBrightness(i);
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayval); // Delay for a period of time (in milliseconds).
  }
  delay(1000);

} // loop



