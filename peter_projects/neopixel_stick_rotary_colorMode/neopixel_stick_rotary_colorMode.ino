// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library


// --- set up rotary encoder

#include <RotaryEncoder.h>

// Setup a RoraryEncoder for pins A2 and A3:
RotaryEncoder encoder(A2, A3);


// --- set up button (switch)

#include <Button.h>

#define BUTTON        7
Button button = Button(BUTTON, PULLUP);


// --- set up neo pixels

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIXEL_PIN     5
#define NUMPIXELS     8

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

int delayval = 20; // delay for half a second

int r = 127;
int g = 127;
int b = 127;

// -- variuos stuff

int colorMode = 0;

#include <EEPROM.h>

struct MyObject {
  int r;
  int g;
  int b;
};

void setup() {
  Serial.begin(57600);
  pixels.begin(); // This initializes the NeoPixel library.
  retreiveRGB();
  showRGB();
}

void loop() {

  if (button.uniquePress() == true) {
    Serial.println("button.uniquePress");
    ++colorMode;
    if (colorMode > 2) {
      colorMode = 0;
    }
    Serial.println(colorMode);

    switch (colorMode) {
      case 0:
        for (int i = 0; i < NUMPIXELS; i++) {
          pixels.setPixelColor(i, pixels.Color(255, 0, 0));
        }

        break;
      case 1:
        for (int i = 0; i < NUMPIXELS; i++) {
          pixels.setPixelColor(i, pixels.Color(0, 255, 0));
        }
        break;
      case 2:
        for (int i = 0; i < NUMPIXELS; i++) {
          pixels.setPixelColor(i, pixels.Color(0, 0, 255));
        }
        break;
    }
    pixels.show();
    delay(500);
    showRGB();
    delay(100);
  }


  // --- interrogate rotary controller
  static int pos = 0;
  encoder.tick();

  int newPos = encoder.getPosition();
  if (pos != newPos) {

    int diff = newPos - pos;

    Serial.println(diff);

    pos = newPos;

    switch (colorMode) {
      case 0:
        r = r + diff;
        if (r < 0) {
          r = 0;
        }
        if (r > 255) {
          r = 255;
        }
        break;
      case 1:
        g = g + diff;
        if (g < 0) {
          g = 0;
        }
        if (g > 255) {
          g = 255;
        }
        break;
      case 2:
        b = b + diff;
        if (b < 0) {
          b = 0;
        }
        if (b > 255) {
          b = 255;
        }
        break;
    }


    Serial.print("r: "); Serial.print(r);
    Serial.print("  g: "); Serial.print(g);
    Serial.print("  b: "); Serial.println(b);


    showRGB();
    storeRGB();

  } // if


  /*
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.

  for (int i = 0; i < NUMPIXELS; i++) {

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(255, 255, 150));

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }

  for (int i = NUMPIXELS; i > 0; i--) {

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(i * 12, 0, 0));

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }
  */
}

void showRGB() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
  pixels.show();
}

void storeRGB() {
  MyObject rgb {
    r, g, b
  };

  int eeAddress = 0;
  EEPROM.put(eeAddress, rgb);
}

void retreiveRGB() {
  MyObject rgb;
  EEPROM.get(0, rgb);
  //  Serial.println(rgb);
  r = rgb.r;
  g = rgb.g;
  b = rgb.b;
}
