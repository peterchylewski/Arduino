// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library


// --- set up rotary encoder

#include <RotaryEncoder.h>

// Setup a RoraryEncoder for pins A2 and A3:
RotaryEncoder encoder(A2, A3);


// --- set up button (switch)

#include <Button.h>

#define BUTTON 2
Button button = Button(BUTTON, PULLUP);


// --- set up neo pixels

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIXEL_PIN            6
#define NUMPIXELS      8

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

int delayval = 20; // delay for half a second


// -- variuos stuff

int colorMode = 0;

void setup() {
  Serial.begin(57600);

  pinMode(9, OUTPUT);    // Use Built-In LED for Indication
  digitalWrite(9, LOW);
  
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {

  if (button.uniquePress() == true) {
    Serial.println("button.uniquePress");
    ++colorMode;
  }


  // --- interrogate rotary controller
  static int pos = 0;
  encoder.tick();

  int newPos = encoder.getPosition();
  if (pos != newPos) {
    Serial.print(newPos);
    Serial.println();
    pos = newPos;

    int luma = newPos * 4;

    if (luma < 0) {
      luma = 0;
    }
    if (luma > 255) {
      luma = 255;
    }

    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(255, luma, luma));
      pixels.show();
    }
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
