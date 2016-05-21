

// see also: https://www.electronicsblog.net/arduino-fm-receiver-with-tea5767/

#include <SPI.h>
#include <Wire.h>

// prepare rotary encoder

int encoder0PinA = 5;
int encoder0PinB = 6;
int encoder0PinClick = 4;
int encoder0Pos = 0;
int encoder0PinALast = HIGH;
int n = LOW;
int c = LOW;
int clickstate = LOW;

// -------------- prepare display

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// prepare radio

// Get TEA5767 library at https://github.com/andykarpov/TEA5767
#include <TEA5767.h>

TEA5767 Radio;
double frequency = 0;
byte status = 0;

void setup() {

  // setup serial
  Serial.begin(115200);

  // -------------- setup rotary encoder

  pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);
  pinMode (encoder0PinClick, INPUT);

  // -------------- setup display

  //display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  display.clearDisplay();

  display.setCursor(6, 0);
  display.setTextSize(8);
  display.println("*FOO*");
  display.display();

  // setup radio

  Wire.begin();
  frequency = 92.6; //starting frequency
  setFrequency();

}

void loop() {

  unsigned char buf[5];
  int stereo;
  int signalLevel;

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
      frequency += .1;
    } else {
      frequency -= .1;
    }
    setFrequency();

    Serial.print(frequency); Serial.print("\n");
    /*
    if (Radio.read_status(buf) == 1) {
        // Get radio data
        signalLevel = (Radio.signal_level(buf) * 100) / 15;
        Serial.print(signalLevel); Serial.print("\n");

        stereo = Radio.stereo(buf);
        Serial.print(stereo); Serial.print("\n");
    }
    */
    //soundController.setVolume(encoder0Pos);
  }
  encoder0PinALast = n;


  display.clearDisplay();
  display.setTextSize(4);
  display.setCursor(6, 0);
  display.setTextSize(4);
  display.println(frequency);
  display.display();

  /*
    display.setCursor(0, 35);
    display.setTextSize(2);
    display.println(signalLevel);

    display.display();
    */
}

void displayInfo() {


}

void setFrequency() {

  Radio.set_frequency(frequency);
  delay(10);

}
