/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/


// http://robotcantalk.blogspot.ch/2015/03/interfacing-arduino-with-ssd1306-driven.html

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11100000, B00000011, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B00000000, B00000000, B00111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11110000, B00000011, B11111000, B00001111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11000000, B01111111, B11111111, B10000001, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B00001111, B11111111, B11111111, B11100000, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111110, B00111111, B00000111, B11111111, B11111000, B01111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111100, B01111111, B10111111, B00111111, B11111110, B00111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111001, B11111111, B11111111, B00111101, B11011111, B00011111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11110011, B11111111, B00111111, B11111111, B11111111, B10000000, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11100111, B11001111, B01111111, B11111111, B11111111, B10000001, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11001100, B11111111, B11111111, B11111111, B11111111, B11000001, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11001111, B10001111, B11111111, B11111111, B11111111, B11111111, B11000001, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11101111, B10011111, B11110000, B00011111, B11111100, B00001111, B11000011, B11011111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11110011, B00111111, B11000000, B00000111, B11110000, B00000001, B11100011, B10011111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111100, B11100011, B00111111, B10001111, B11100001, B11100011, B11111000, B01110001, B00110000, B00011111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111000, B00000000, B01111111, B00111111, B11111001, B11000111, B11111110, B01110000, B00000000, B00001111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111100, B00011000, B01111110, B01111111, B11111100, B11001111, B11111111, B00110000, B10001111, B10000111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11001100, B01111100, B11111111, B11111110, B00001111, B11111111, B10010001, B00111111, B11100011, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11100000, B11111000, B11111111, B11111110, B00011111, B11111111, B11010000, B01111111, B11110001, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11110000, B11111001, B11111111, B11111110, B00011111, B11111111, B11001100, B11110001, B11110001, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11100000, B00110000, B11110011, B11111111, B11111111, B00011111, B11111111, B11001101, B11100000, B00011001, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11100000, B00000000, B11110011, B11111111, B00011111, B00011111, B01111111, B11101101, B11000000, B00011000, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B10000000, B00000000, B11100111, B11111111, B00011111, B00011110, B00111111, B11101111, B11000000, B00001000, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B10001111, B11100001, B11100111, B11111111, B11111111, B00011111, B11111111, B11101111, B10000000, B00001100, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B00011111, B11110001, B11100111, B11111111, B11111110, B00011111, B11111111, B11101111, B10000000, B00100100, B01111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B00111111, B11111001, B11100111, B11111111, B11111110, B10001111, B11111111, B11101111, B10000001, B00100100, B01111111, B11111111, B11111111,
B11111111, B11111111, B11111110, B00110000, B00111101, B11100111, B11111111, B11111100, B10001111, B11111111, B11011111, B10011000, B10001100, B01111111, B11111111, B11111111,
B11111111, B11111111, B11111110, B01100000, B00011111, B11100111, B11111111, B11111001, B11000111, B11111111, B10011111, B10011100, B10001100, B01111111, B11111111, B11111111,
B11111111, B11111111, B11111110, B01000000, B00001111, B11100011, B11111111, B11110001, B11100011, B11111111, B10011111, B10011100, B00001100, B01111111, B11111111, B11111111,
B11111111, B11111111, B11111100, B01000000, B00000111, B11100011, B11111111, B11000011, B11110000, B11111111, B00011111, B11101100, B00111100, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111100, B01001000, B00000111, B11110001, B11111111, B10000011, B11111000, B00011110, B00111111, B11100100, B01111100, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111100, B11111100, B00000111, B11111000, B01111100, B00000111, B11111100, B00000000, B01111111, B10000000, B11111000, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111100, B11111000, B11111111, B11111100, B00000000, B00000111, B11111110, B00000001, B11111111, B00000001, B11111001, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111100, B01111000, B11111111, B11111110, B00000000, B10001111, B11111111, B00001111, B11111111, B00000011, B11110001, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111100, B01111000, B01110011, B11111111, B11111111, B10011111, B11111111, B10001111, B11111111, B00110000, B00000011, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111110, B01111000, B00000011, B11111111, B11111111, B00011111, B11111111, B10001111, B11111111, B11100000, B00000111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111110, B00111000, B00001111, B11111111, B11111111, B00111111, B11111111, B11000111, B11111111, B11000000, B00011111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111110, B00011100, B01000111, B11111111, B11111110, B01111111, B11111111, B11000111, B11111111, B10001111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B00011111, B10000001, B11111111, B11111110, B01111111, B11111111, B11100111, B11111111, B10001111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B00001110, B00000001, B11111111, B11111110, B11111111, B11111111, B11100011, B11111111, B10000011, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B10000000, B00010001, B11111111, B11111100, B11111111, B11111111, B11110011, B11111111, B10000011, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11000000, B00111001, B11111111, B11111100, B11111111, B11111111, B11110001, B11111100, B10000011, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11110000, B01110001, B11011111, B11111101, B11111111, B11111111, B11111001, B11111001, B11000011, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111110, B11110001, B11001111, B11111001, B11110111, B11111111, B11111000, B11111001, B11000011, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111110, B11000001, B11100111, B11111000, B11111111, B11111111, B11111100, B11111001, B11100011, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111100, B11000011, B11100111, B11111000, B11111111, B11111111, B11111100, B11110001, B11100001, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111110, B11000011, B11100111, B11111001, B11111111, B11111111, B11111100, B11111001, B11000001, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111101, B11000011, B11000111, B11110011, B11111111, B11111111, B11111110, B01111000, B10001001, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111101, B11000001, B10001111, B11110011, B11111111, B11111111, B11111110, B01111000, B00001101, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111101, B11000000, B00011111, B11110011, B11111111, B11111111, B11111110, B01111000, B00011101, B11111111, B11111111, B11111111, B11111111
};

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


void setup()   {
  Serial.begin(9600);
  Serial.print("foo");
    
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  //display.begin(SSD1306_SWITCHCAPVCC, 0x3D);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // init done

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.

  display.clearDisplay();
  display.display();
  //delay(2000);


}


void loop() {

  // miniature bitmap display
  display.clearDisplay();
  display.drawBitmap(0, 0,  logo16_glcd_bmp, 128, 64, 1);
  display.display();
  delay(10000);
  display.clearDisplay();

  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println("   TEXT\nMACHINES");
  display.display();
  delay(2000);
  display.clearDisplay();

  // draw the first ~12 characters in the font
  testdrawchar();
  display.display();
  delay(2000);
  display.clearDisplay();

  // draw scrolling text
  /*
  testscrolltext();
  delay(2000);
  display.clearDisplay();
*/

  // text display tests

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Hello, world!");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.println(3.141592);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("0x"); display.println(0xDEADBEEF, HEX);
  display.display();
  delay(2000);
  display.clearDisplay();



  // draw many lines
  testdrawline();
  display.display();
  delay(2000);
  display.clearDisplay();

  // draw rectangles
  testdrawrect();
  display.display();
  delay(2000);
  display.clearDisplay();

  // draw multiple rectangles
  testfillrect();
  display.display();
  delay(2000);
  display.clearDisplay();

  // draw mulitple circles
  testdrawcircle();
  display.display();
  delay(2000);
  display.clearDisplay();

  // draw a white circle, 10 pixel radius
  display.fillCircle(display.width() / 2, display.height() / 2, 10, WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();

  testdrawroundrect();
  delay(2000);
  display.clearDisplay();

  testfillroundrect();
  delay(2000);
  display.clearDisplay();

  testdrawtriangle();
  delay(2000);
  display.clearDisplay();

  testfilltriangle();
  delay(2000);
  display.clearDisplay();






  // invert the display
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);

  // draw a bitmap icon and 'animate' movement
  // testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
}


void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  uint8_t icons[NUMFLAKES][3];

  // initialize
  for (uint8_t f = 0; f < NUMFLAKES; f++) {
    icons[f][XPOS] = random(display.width());
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random(5) + 1;

    Serial.print("x: ");
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(" y: ");
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(" dy: ");
    Serial.println(icons[f][DELTAY], DEC);
  }

  while (1) {
    // draw each icon
    for (uint8_t f = 0; f < NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, WHITE);
    }
    display.display();
    delay(200);

    // then erase it + move it
    for (uint8_t f = 0; f < NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS],  logo16_glcd_bmp, w, h, BLACK);
      // move it
      icons[f][YPOS] += icons[f][DELTAY];
      // if its gone, reinit
      if (icons[f][YPOS] > display.height()) {
        icons[f][XPOS] = random(display.width());
        icons[f][YPOS] = 0;
        icons[f][DELTAY] = random(5) + 1;
      }
    }
  }
}


void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  for (uint8_t i = 0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.println();
  }
  display.display();
}

void testdrawcircle(void) {
  for (int16_t i = 0; i < display.height(); i += 2) {
    display.drawCircle(display.width() / 2, display.height() / 2, i, WHITE);
    display.display();
  }
}

void testfillrect(void) {
  uint8_t color = 1;
  for (int16_t i = 0; i < display.height() / 2; i += 3) {
    // alternate colors
    display.fillRect(i, i, display.width() - i * 2, display.height() - i * 2, color % 2);
    display.display();
    color++;
  }
}

void testdrawtriangle(void) {
  for (int16_t i = 0; i < min(display.width(), display.height()) / 2; i += 5) {
    display.drawTriangle(display.width() / 2, display.height() / 2 - i,
                         display.width() / 2 - i, display.height() / 2 + i,
                         display.width() / 2 + i, display.height() / 2 + i, WHITE);
    display.display();
  }
}

void testfilltriangle(void) {
  uint8_t color = WHITE;
  for (int16_t i = min(display.width(), display.height()) / 2; i > 0; i -= 5) {
    display.fillTriangle(display.width() / 2, display.height() / 2 - i,
                         display.width() / 2 - i, display.height() / 2 + i,
                         display.width() / 2 + i, display.height() / 2 + i, WHITE);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}

void testdrawroundrect(void) {
  for (int16_t i = 0; i < display.height() / 2 - 2; i += 2) {
    display.drawRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i, display.height() / 4, WHITE);
    display.display();
  }
}

void testfillroundrect(void) {
  uint8_t color = WHITE;
  for (int16_t i = 0; i < display.height() / 2 - 2; i += 2) {
    display.fillRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i, display.height() / 4, color);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}

void testdrawrect(void) {
  for (int16_t i = 0; i < display.height() / 2; i += 2) {
    display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i, WHITE);
    display.display();
  }
}

void testdrawline() {
  for (int16_t i = 0; i < display.width(); i += 4) {
    display.drawLine(0, 0, i, display.height() - 1, WHITE);
    display.display();
  }
  for (int16_t i = 0; i < display.height(); i += 4) {
    display.drawLine(0, 0, display.width() - 1, i, WHITE);
    display.display();
  }
  delay(250);

  display.clearDisplay();
  for (int16_t i = 0; i < display.width(); i += 4) {
    display.drawLine(0, display.height() - 1, i, 0, WHITE);
    display.display();
  }
  for (int16_t i = display.height() - 1; i >= 0; i -= 4) {
    display.drawLine(0, display.height() - 1, display.width() - 1, i, WHITE);
    display.display();
  }
  delay(250);

  display.clearDisplay();
  for (int16_t i = display.width() - 1; i >= 0; i -= 4) {
    display.drawLine(display.width() - 1, display.height() - 1, i, 0, WHITE);
    display.display();
  }
  for (int16_t i = display.height() - 1; i >= 0; i -= 4) {
    display.drawLine(display.width() - 1, display.height() - 1, 0, i, WHITE);
    display.display();
  }
  delay(250);

  display.clearDisplay();
  for (int16_t i = 0; i < display.height(); i += 4) {
    display.drawLine(display.width() - 1, 0, 0, i, WHITE);
    display.display();
  }
  for (int16_t i = 0; i < display.width(); i += 4) {
    display.drawLine(display.width() - 1, 0, i, display.height() - 1, WHITE);
    display.display();
  }
  delay(250);
}

void testscrolltext(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.clearDisplay();
  display.println("scroll");
  display.display();

  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
}
