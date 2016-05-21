

// -------------- prepare gps

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2);
Adafruit_GPS GPS(&mySerial);

// Forward declarations
void GPS_Setup();
void GPS_Tick();

// -------------- prepare display

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// Timer
uint32_t timer = millis();

// display toggle
boolean displayToggle = false;

void setup() {

  // -------------- set up serial

  // connect at 115200 so we can read the GPS fast enuf and
  // also spit it out

  Serial.begin(115200);
  Serial.println("Adafruit GPS logging start test!");

  // -------------- set up gps

  GPS_Setup();

  //GPS.sendCommand(PGCMD_ANTENNA);

  // -------------- set up oled display

  display.begin(SSD1306_SWITCHCAPVCC);

  display.clearDisplay();   // Clear the buffer.
  display.setTextColor(WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("foo");

  display.setCursor(0, 10);
  display.println(PMTK_Q_RELEASE);

  display.display();
}

void loop() {

  GPS_Tick();

  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if ((c)) {
    Serial.write('Serial.read() '); Serial.write(c);
  }

  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    // Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }

  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis())  timer = millis();

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer

    display.clearDisplay();
    //display.display();

    display.setCursor(120, 0);

    if (displayToggle == true) {
      display.print("*");
    } else {
      display.print(" ");
    }
    display.display();

    displayToggle = !displayToggle;
    Serial.print("\displayToggle: "); Serial.println((bool)displayToggle);


    display.setCursor(0, 0);
    display.print("Lat: "); display.print(GPS.lat); display.print(GPS.latitude, 8);

    display.setCursor(0, 10);
    display.print("Lon: "); display.print(GPS.lon); display.print(GPS.longitude, 8);

    if (GPS.fix <= 0) {
      display.setCursor(0, 20);
      display.print("*NO FIX*");
    }

    display.setCursor(72, 20);
    display.print(GPS.hour, DEC); display.print(':');
    display.print(GPS.minute, DEC); display.print(':');
    display.print(GPS.seconds, DEC);

    display.display();

    Serial.print("\nTime: ");
    Serial.print(GPS.hour, DEC); Serial.print(':');
    Serial.print(GPS.minute, DEC); Serial.print(':');
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);

    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);

      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    }
  }



}


void GPS_Setup()
{
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  // required for number of SATS and altitude.
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

  // uncomment this line to turn on only the "minimum recommended" data
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time

  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  // ************************************************************
  GPS.sendCommand(PGCMD_ANTENNA);  // required to get PGTOP working
  // Commnets: brycej date: 02/18/2014
  // I think this is incorrect and needs to be updated in the header files
  // FORMAT: $PGTOP,11,value*checksum
  //    1. Active Antenna Shorted
  //    2. Using Internal Antenna $PGTOP,11,2*6E
  //    3. Using Active Antenna   $PGTOP,11,3*6F
  //
  //GPS.sendCommand(PGTOP_ANTENNA);
  //delay(500);

  // Ask for firmware version
  //mySerial.println(PMTK_Q_RELEASE);
  //delay(1000);
}

// Read in and tick the GPS class
void GPS_Tick()
{

  // 'hand query' the GPS for new data
  // read data from the GPS in the 'main loop'
  char c = GPS.read();

  if ((c))
    Serial.write(c);

  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived())
  {
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }
}

