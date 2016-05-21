

// -------------- prepare led

int led = 13;

// -------------- prepare gps

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(8, 1);
Adafruit_GPS GPS(&mySerial);

#include <SPI.h>
#include <Wire.h>

// -------------- prepare display

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// Timer
uint32_t timer = millis();

void setup() {

  pinMode(led, OUTPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  display.clearDisplay();
  
  display.setCursor(6, 0);
  display.setTextSize(2);
  display.println("*SUPERGPS*");
  display.display();
  display.setTextSize(1);

  GPS_Setup();

}

// steinbühlallee 7.5565392000000005,47.5460134,0.0 7.5569844,47.5472409,0.0
// mein balkon 7.556521099999999,47.5459718,0.0 7.556664600000001,47.5463547,0.0

void loop() {

  //GPS_Tick();

  char c = GPS.read();

  // if you want to debug, this is a good time to do it!
  if ((c)) {
    Serial.write('Serial.read() '); Serial.write(c);
  }



  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis())  timer = millis();

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 100) {

    timer = millis(); // reset the timer

    // blink led
       if (millis() - timer > 10) {
        digitalWrite(led, HIGH);
      }
      
      if (millis() - timer > 20) {
        digitalWrite(led, LOW);
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

    // https://github.com/openhomeautomation/arduino-geofencing/blob/master/geo_fencing_io/geo_fencing_io.ino

    display.clearDisplay();
    
    display.setCursor(0, 0);
    display.print("Lat: "); display.print(GPS.lat); display.print(GPS.latitude, 8);

    display.setCursor(0, 9);
    display.print("Lon: "); display.print(GPS.lon); display.print(GPS.longitude, 8);
 
    display.setCursor(0, 18);
    display.print("Knots: "); display.print(GPS.speed); display.print(" Alt: "); display.print(GPS.altitude); display.print(" Angle: "); display.print(GPS.angle);
    
    display.setCursor(0, 36);
    display.print("Sat: "); display.print(GPS.satellites, DEC);
    display.print(" Q: ");  display.print(GPS.fixquality, DEC);
    if (GPS.fix <= 0) {
      display.print(" *NO FIX");
    } else {
      display.print(" *FIX  ");
    }

    display.setCursor(0, 45);
    display.print("Time: ");
    if (GPS.hour < 10)      {
      display.print('0');
    } display.print(GPS.hour, DEC); display.print(':');
    if (GPS.minute < 10)    {
      display.print('0');
    } display.print(GPS.minute, DEC); display.print(':');
    if (GPS.seconds < 10)   {
      display.print('0');
    } display.print(GPS.seconds, DEC);

    display.print('.');
    if (GPS.milliseconds < 100)      {
      display.print('00');
    } else if (GPS.milliseconds < 10) {
       display.print('0');
    }
    display.print(GPS.milliseconds, DEC);

    display.setCursor(0, 54);
    display.print("Date: ");
    if (GPS.day < 10)      {
      display.print('0');
    }
    display.print(GPS.day, DEC); display.print('.');
    if (GPS.month < 10)      {
      display.print('0');
    }
    display.print(GPS.month, DEC); display.print('.');
    display.print(GPS.year, DEC);

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
    Serial.print(" satellites: "); Serial.println((int)GPS.satellites);

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

    //GPS.begin(57600);
    //GPS.sendCommand(PMTK_SET_BAUD_57600);


  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  // required for number of SATS and altitude.
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time

    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_ALLDATA);


  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);   // 10 Hz update rate
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

  delay(1000);
  // Ask for firmware version
  GPS.sendCommand(PMTK_Q_RELEASE);
  delay(1000);
}

// Read in and tick the GPS class
void GPS_Tick()
{

  // 'hand query' the GPS for new data
  // read data from the GPS in the 'main loop'
  char c = GPS.read();

  if ((c)) {
    Serial.write(c);
  }

  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived())
  {
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }
}

