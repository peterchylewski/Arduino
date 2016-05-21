/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */

#include <Arduino.h>

// -------------- prepare serial

#define USE_SERIAL Serial

// -------------- prepare wi-fi (client)

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

ESP8266WiFiMulti WiFiMulti;

// -------------- prepare JSON parser

#include <ArduinoJson.h>

StaticJsonBuffer<200> jsonBuffer;

// -------------- prepare display

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 3
Adafruit_SSD1306 display(OLED_RESET);

void setup() {

  // display

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  display.setTextColor(WHITE);
  display.setCursor(6, 0);
  display.setTextSize(2);
  display.println("*HUZZAHH*");
  display.display();
  display.setTextSize(1);

  // serial

  USE_SERIAL.begin(115200);
  // USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  /*
  for (uint8_t t = 4; t > 0; t--) {
  USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
  USE_SERIAL.flush();
  delay(100);
  }
  */

  // wi-fi

  WiFiMulti.addAP("tofu", "82wv-bmtb-1h8j-udwd");


}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    HTTPClient http;

    USE_SERIAL.print("[HTTP] begin...\n");
    // configure traged server and url
    //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
    //http.begin("http://192.168.1.114/json_proxy/index.php"); //HTTP
    //http.begin("http://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20weather.forecast%20where%20woeid%20in%20(select%20woeid%20from%20geo.places(1)%20where%20text%3D%22nome%2C%20ak%22)&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys"); //HTTP

    http.begin("http://query.yahooapis.com/v1/public/yql?q=select%20wind%20from%20weather.forecast%20where%20woeid%20in%20(select%20woeid%20from%20geo.places(1)%20where%20text%3D%22basel%2C%20ch%22)&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys");

    //http.begin("http://data.netcetera.com/smn/smn");
    
    USE_SERIAL.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        USE_SERIAL.println(payload);

        StaticJsonBuffer<1000> jsonBuffer;
        JsonObject& root = jsonBuffer.parseObject(payload);
        if (!root.success()) {
          USE_SERIAL.println("parseObject() failed");
          //return;
        } else {
          String foo = root["query"]["created"];
          USE_SERIAL.println(foo);
          display.clearDisplay();
          writeToDisplay(foo, 0, 30);
        }
      }
    } else {
      USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(10000);
}


void writeToDisplay(String s, int x, int y) {
  display.setCursor(x, y);
  display.setTextSize(1);
  display.println(s);
  display.display();
}
