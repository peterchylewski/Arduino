

#define DEBUG true

// -------------- prepare LED

#define LED 0


// -------------- prepare wi-fi

#include <ESP8266WiFi.h>

#define WIFI_SSID "tofu"
#define WIFI_PSK  "82wv-bmtb-1h8j-udwd"

WiFiClient client;

// -------------- prepare display

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 3
Adafruit_SSD1306 display(OLED_RESET);

// -------------- prepare button

#include <Button.h>
Button button = Button(2, PULLUP);

#include <ArduinoJson.h>

void setup() {

  if (DEBUG == true) {
    Serial.begin(9600);
  }

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);



  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  /*

  display.setTextColor(WHITE);
  display.setCursor(6, 0);
  display.setTextSize(2);
  display.println("*HUZZAHH*");
  */
  display.display();
  display.setTextSize(1);

  startNetwork();

}

void loop() {

  if (button.uniquePress() == true) {
    display.clearDisplay();
    display.setCursor(6, 0);
    display.println("*foo*");
    display.display();

    delay(1000);
    display.clearDisplay();
    display.display();
    sendRequest();
  }
}


void writeToDisplay(String s, int x, int y) {
  display.setCursor(x, y);
  display.setTextSize(1);
  display.println(s);
  display.display();
}

void startNetwork() {

  // We start by connecting to a WiFi network

  if (DEBUG == true) {
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(WIFI_SSID);
  }

  WiFi.begin(WIFI_SSID, WIFI_PSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  for (int i = 0; i < 3; i++) {
    digitalWrite(LED, LOW); delay(100);
    digitalWrite(LED, HIGH); delay(100);
  }
  digitalWrite(LED, HIGH);

  if (DEBUG == true) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }

  sendRequest();
}

void sendRequest() {

  if (DEBUG == true) {
    Serial.println("sendRequest");
  }

  const String uri = "/airport/status/sfo?format=json";
  const char http_site[] = "services.faa.gov";
  const int http_port = 80;

  if (!client.connect(http_site, http_port)) {
    if (DEBUG == true) {
      Serial.println("connection failed");
    }
    return;

  }

  Serial.println("connection successful");

  ////String site = "services.faa.gov";
  //String path = "/airport/status/sfo?format=json";

  String site = "services.faa.gov";
  String path = "/airport/status/sfo?format=json";

  String cmd = "";
  cmd += "GET " + path + " HTTP/1.1\r\n";
  cmd += "Host: " + site + "\r\n";
  cmd += "Connection: close\r\n\r\n";

  Serial.print("cmd: "); Serial.println("-------------------");
  Serial.println(cmd);
  Serial.println("-------------------");

  client.print(cmd);

  // Make an HTTP GET request
  /*
  client.println("GET " + uri + " HTTP/1.1");
  client.print("Host: ");
  client.println(http_site);
  client.print("Connection: keep-alive\r\n\r\n");
*/

  //StaticJsonBuffer<200> jsonBuffer;

  while (client.available()) {
    String data = client.readStringUntil('\r');

    Serial.print(data);
    //delay(100);
    /*
    JsonObject& root = jsonBuffer.parseObject(data);
    // Test if parsing succeeds.
    if (!root.success()) {
      Serial.println("parseObject() failed");

    } else {
        Serial.println("parseObject() succeeded");
    }
    */
  }

}


