

// -------------- prepare gps

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

Adafruit_GPS GPS(&Serial1);

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

void setup() {

  // -------------- set up serial

  // connect at 115200 so we can read the GPS fast enuf and
  // also spit it out
  /*
  Serial.begin(115200);
  while (!Serial);
  delay(1000);
  Serial.println("Adafruit GPS logging start test!");
  */


  // -------------- set up gps

 GPS_Setup();

  // -------------- set up oled display

  display.begin(SSD1306_SWITCHCAPVCC);

  display.clearDisplay();   // Clear the buffer.
  display.setTextColor(WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("foo");

  display.setCursor(0, 10);
  display.println("bar");

  display.display();
}

void loop() {
  GPS_Tick();


  // 1Hz update rate
  if (millis() - timer > 1000)
  {

    display.clearDisplay();

    display.setCursor(0, 0);
    display.print("Lat: "); display.print(GPS.latitude, 8);

    display.setCursor(0, 10);
    display.print("Lon: "); display.print(GPS.longitude, 8);

    display.setCursor(0, 20);
    if (GPS.fix <= 0) {
      display.print("no fix");
    }

    display.setCursor(72, 20);
    display.print(GPS.hour, DEC); display.print(':');
    display.print(GPS.minute, DEC); display.print(':');
    display.print(GPS.seconds, DEC);

    display.display();
  }


}


void GPS_Setup()
{
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  // required for number of SATS and altitude.
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
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


  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived())
  {
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }
}

