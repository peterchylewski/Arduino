
#define DEBUG false

#include <SPI.h>
#include <Wire.h>

// ---prepare rotary encoder

#include <RotaryEncoder.h>

// Setup a RoraryEncoder for pins A2 and A3:
RotaryEncoder encoder(A2, A3);


// --- prepare button (switch)

#include <Button.h>

#define BUTTON        7
Button button = Button(BUTTON, PULLUP);


// --- prepare neo pixels

#include <Adafruit_NeoPixel.h>

#define PIXEL_PIN      6
#define NUMPIXELS      8

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

// --- prepare display

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// --- prepare radio

#include <TEA5767.h>
TEA5767 Radio;
double frequency = 0;

// -- div
unsigned long timer = 0;


void setup() {

  if (DEBUG == true) { Serial.begin(9600); }

  /*
  // setup pixels
  pixels.begin(); // This initializes the NeoPixel library.
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(25, 25, 25));
  }
  pixels.show(); // This sends the updated pixel color to the hardware.
  */

  // -------------- setup display

  setupDisplay();
  

  // setup radio
  Wire.begin();
  frequency = 90.6; //starting frequency
  Radio.set_frequency(frequency);
  showStatus();

  timer = millis();
}

void loop() {
  // put your main code here, to run repeatedly:

  
  unsigned long current_millis = millis();
  if (current_millis - timer > 1000) {
    showStatus();
    timer = millis();
  }
 

  if (button.uniquePress() == true) {
    Serial.println("button.uniquePress");
  }
  
  // --- interrogate rotary controller
  static int pos = 0;
  static int diff = 0;

  encoder.tick();
  int newPos = encoder.getPosition();
  if (pos != newPos) {
    diff = newPos - pos;
    
    if (diff == -1) {
      frequency -= .1;
    } else {
      frequency += .1;
    }
    if (DEBUG == true) {
      Serial.println(frequency);
    }
    Radio.set_frequency(frequency);

    display.clearDisplay();
    showFrequency(frequency);
    display.display();
    timer = millis() + 500;
    
    pos = newPos;
  } // if
}

void setupDisplay() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  display.clearDisplay();

  display.setCursor(6, 0);
  display.setTextSize(8);
  display.println("*FOO*");
  display.display();

}

void showFrequency(double frequency) {
    
    display.setTextSize(4);
    display.setCursor(6, 0);
    display.setTextSize(4);
    display.println(frequency);
    
}

void showStatus() {
  unsigned char buf[5];
  int stereo;
  int signal_level;
  double current_freq;
  unsigned long current_millis = millis();

  if (Radio.read_status(buf) == 1) {
    current_freq =  floor (Radio.frequency_available (buf) / 100000 + .5) / 10;
    stereo = Radio.stereo(buf);
    signal_level = Radio.signal_level(buf);

    if (DEBUG == true) {
      Serial.print("FM: "); Serial.print(current_freq);
      if (stereo) {
        Serial.print (" STEREO ");
      } else {
        Serial.print(" MONO ");
      }
      Serial.print(" LEVEL: ");
      Serial.println(signal_level);
    }

    display.clearDisplay();
    
  showFrequency(current_freq);

    display.setCursor(0, 34);
    display.setTextSize(2);
    if (stereo == 1) {
      display.println(" *STEREO*");
    } else {
      display.println("   mono");
    }


    display.fillRect(0, 54, signal_level * 10, 4, 1);
    
    display.display();

    /*
        for (int i = 0; i < NUMPIXELS; i++) {
          pixels.setPixelColor(i, pixels.Color(0, 0, 0));
        }

        for (int i = 0; i < signal_level; i++) {
          if (stereo) {
            pixels.setPixelColor(i, pixels.Color(0, 25, 0));
          } else {
            pixels.setPixelColor(i, pixels.Color(25, 0, 0));
          }
        }
        pixels.show();
    */
  }

}

