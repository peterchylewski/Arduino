
// neopixel stuff

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define NUMPIXELS      1
#define PIN            8

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// encoder stuff

int val;
int encoder0PinA = 5;
int encoder0PinB = 6;
int encoder0PinClick = 7;
int encoder0Pos = 0;
int encoder0PinALast = LOW;
int n = LOW;

void setup() {
   

  // encoder stuff
    
  pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);
  pinMode (encoder0PinClick, INPUT);
  Serial.begin(9600);
}

void loop() {


  // encoder stuff
    
  n = digitalRead(encoder0PinA);
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encoder0PinB) == LOW) {
      encoder0Pos--;
    } else {
      encoder0Pos++;
    }
    Serial.print (encoder0Pos);
    Serial.print ("\n");
  }
  encoder0PinALast = n;
}
