

// --- prepare rotary encoder

#include <RotaryEncoder.h>

// Setup a RoraryEncoder for pins A2 and A3:
RotaryEncoder encoder(A2, A3);


// --- prepare button (switch)

#include <Button.h>

#define BUTTON        7
Button button = Button(BUTTON, PULLUP);

// --- prepare neopixels

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIXEL_PIN     10
#define NUMPIXELS     12

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

int delayval = 20;

int r = 127;
int g = 127;
int b = 127;

uint32_t white = pixels.Color(248, 233, 127);
uint32_t black = pixels.Color(0, 0, 0);
uint32_t red = pixels.Color(255, 0, 0);
uint32_t green = pixels.Color(0, 255, 0);
uint32_t blue = pixels.Color(0, 0, 255);
uint32_t darkOrange = pixels.Color(204, 61, 0);

struct MyObject {
  int r;
  int g;
  int b;
};

// -- variuos stuff

int colorMode = 0;

#include <EEPROM.h>

void setup() {
  
  Serial.begin(9600);
  
  pixels.begin(); // This initializes the NeoPixel library.

  retreiveRGB();
  setAllPixelsRGB(r, g, b);

  int n = getPixelNum(0, 0);
  pixels.setPixelColor(n, white);

  n = getPixelNum(3, 2);
  pixels.setPixelColor(n, white);

  pixels.show();

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
          pixels.setPixelColor(i, red);
        }
        break;
      case 1:
        for (int i = 0; i < NUMPIXELS; i++) {
          pixels.setPixelColor(i, green);
        }
        break;
      case 2:
        for (int i = 0; i < NUMPIXELS; i++) {
          pixels.setPixelColor(i, blue);
        }
        break;
    }
    pixels.show();
    delay(500);
    setAllPixelsRGB(r, g, b);
    delay(100);
  }


  // --- interrogate rotary controller
  static int pos = 0;
  encoder.tick();

  int newPos = encoder.getPosition();
  if (pos != newPos) {
    int diff = newPos - pos;
    //Serial.println(diff);
    moveAllPixelColors(colorMode, diff);
    
    pos = newPos;

    Serial.println(pos);
    /*
    int i;
    for (i = 0; i < pos; i++) {
      pixels.setPixelColor(i, white);
    }
    for (i = pos; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, darkOrange);
    }
    */
    pixels.show();

  } // end loop


}

void moveAllPixelColors(int mode, int diff) {
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

  setAllPixelsRGB(r, g, b);
  storeRGB();

}

void interrogateRotaryController() {

}

int getPixelNum(int x, int y) {
  int n;
  switch (y) {
    case 0:
      n = 3 - x;
      break;
    case 1:
      n = 4 + x;
      break;
    case 2:
      n = 11 - x;
      break;
  }
  return n;
}


void setAllPixelsRGB(int r, int g, int b) {
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
