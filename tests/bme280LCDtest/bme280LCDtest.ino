/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 8);

#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16



#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1021)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);

void setup() {
  Serial.begin(9600);
  Serial.println(F("BME280 test"));

  display.begin();
  display.setContrast(50);
  display.clearDisplay();
  display.display();

  display.setTextColor(BLACK, WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("welcome");
  display.display();

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

void loop() {

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("T:  "); display.print(bme.readTemperature()); display.println(" *C");
  display.print("H:  "); display.print(bme.readHumidity()); display.println(" %");
  display.print("P: "); display.print(bme.readPressure() / 100.0F); display.println(" hPa");
  display.print("A: "); display.print(bme.readAltitude(SEALEVELPRESSURE_HPA)); display.println(" m");
  

  Serial.print("{\"temp\": "); Serial.print(bme.readTemperature()); Serial.print(", ");
  Serial.print("\"pressure\": "); Serial.print(bme.readPressure() / 100.0F); Serial.print(", ");
  Serial.print("\"humidity\": "); Serial.print(bme.readHumidity()); Serial.print(", ");
  Serial.print("\"altitude\": "); Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA)); Serial.println("}");
  
/*
 Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
  
  Serial.print("Pressure = ");

  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
*/

  display.display();
  delay(1000);
}
