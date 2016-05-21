

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

  const char http_site[] = "192.168.1.115";
  const int http_port = 80;

  if (!client.connect(http_site, http_port)) {
    if (DEBUG == true) {
      Serial.println("connection failed");
    }
    return;
  }

//Serial.println(client.isConnected());
  

  Serial.println("connection successful");

  // Make an HTTP GET request
  client.println("GET /api/peterchylewski/lights/1 HTTP/1.1");
  client.print("Host: ");
  client.println(http_site);
  client.print("Connection: close\r\n\r\n");
 // client.print("Connection: keep-alive\r\n\r\n");

  //StaticJsonBuffer<200> jsonBuffer;

  Serial.println("reading data...");

  while (client.available()) {

    Serial.println("client is avalaible");
    
    String data = client.read(buffer, 300);

    Serial.println(data);
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

  Serial.println();
  Serial.println("closing connection");

}


