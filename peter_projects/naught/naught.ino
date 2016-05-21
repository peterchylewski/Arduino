
#include <SPI.h>
#include <Wire.h>


// ---prepare rotary encoder

#include <RotaryEncoder.h>

// Setup a RoraryEncoder for pins A2 and A3:
RotaryEncoder encoder(A2, A3);


// --- prepare radio

#include <TEA5767.h>
TEA5767 Radio;
double frequency = 0;

void setup() {
  Serial.begin(9600);

  // setup radio

  Wire.begin();
  frequency = 92.6; //starting frequency
  Radio.set_frequency(frequency);
  showStatus();
}

void loop() {
  // put your main code here, to run repeatedly:


  // --- interrogate rotary controller
  static int pos = 0;
  static int diff = 0;

  encoder.tick();

  int newPos = encoder.getPosition();
  if (pos != newPos) {
    //Serial.print(newPos);
    Serial.println(pos - newPos);

    diff = newPos - pos;

    pos = newPos;

    if (diff == -1) {
      frequency -= .1;
    } else {
      frequency += .1;
    }

    Serial.println(frequency);
    Radio.set_frequency(frequency);

    showStatus();
    
  } // if




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
    //lcd.setCursor(0,0);
    //lcd.print("FM: "); lcd.print(current_freq);
    //lcd.setCursor(0,1);

    Serial.print("FM: "); Serial.print(current_freq);
    if (stereo) {
      Serial.print (" STEREO ");
    } else {
      Serial.print(" MONO ");
    }

    Serial.print(" LEVEL: ");
    Serial.println(signal_level);

    //lcd.print(signal_level);

    //Serial.println(current_freq);
  }

}

