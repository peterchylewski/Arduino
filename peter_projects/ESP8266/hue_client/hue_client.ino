/*
*  Simple HTTP get webclient test
*/

#define DEBUG false

/****************************** WiFi ******************************************/

#include <ESP8266WiFi.h>

const char* ssid     = "tofu";
const char* password = "82wv-bmtb-1h8j-udwd";

const char* host = "192.168.1.115";

// Use WiFiClient class to create TCP connections
WiFiClient client;

/****************************** LED ******************************************/

#define LED 0

/****************************** Button ******************************************/

#include <Button.h>

#define BUTTON 2
Button button = Button(BUTTON, PULLUP);

// states
int lampToggle = 0;

void setup() {

  //set LED
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  // set button pin as an input
  //pinMode(BUTTON, INPUT_PULLUP);

  if (DEBUG == true) {
    Serial.begin(9600);
  }

  startNetwork();

} // setup (end)

void loop() {
    
  if (button.uniquePress() == true) {
    Serial.println("button.uniquePress");
    if (DEBUG == true) {
      Serial.print("client available: "); Serial.println(client.available());
      Serial.print("lampToggle: "); Serial.println(lampToggle);
    }
    if (lampToggle == 0) {
      sendData("{\"bri\": 255, \"on\": true, \"ct\": 400, \"transitiontime\": 0}");
    } else {
      sendData("{\"on\": false, \"transitiontime\": 0}");
    }
    lampToggle = (lampToggle == 0 ? 1 : 0);
  }
} // loop (end)


void startNetwork() {

  // Connecting to a WiFi network

  if (DEBUG == true) { Serial.println(); Serial.print("Connecting to "); Serial.println(ssid); }

  WiFi.begin(ssid, password);

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
}

void getData() {


}

void sendData(String JSONString) {
  if (DEBUG == true) { Serial.println("sendData"); Serial.print("connecting to "); Serial.println(host); }

  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    if (DEBUG == true) {
      Serial.println("connection failed");
    }
    return;
  }

  String url = "/api/peterchylewski/groups/0/action/";

  if (DEBUG == true) {
    Serial.print("Requesting URL: "); Serial.println(url);
  }
  //String JSONString = "{\"hue\": 10000}";
  //String JSONString = "{\"on\": true, \"transitiontime\": 0}";

  // This will send the request to the server
  client.print("PUT " + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               //"Connection: close\r\n" +
               "Connection: keep-alive\r\n" +
               "Content-Length: " + JSONString.length() + "\r\n" +
               "Content-Type: application/json" + "\r\n" +
               "\r\n" +
               JSONString
              );

  //delay(500);

  // Read all the lines of the reply from server and print them to Serial
  if (DEBUG == true) {
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    Serial.println(); Serial.println("closing connection");
  }
}

