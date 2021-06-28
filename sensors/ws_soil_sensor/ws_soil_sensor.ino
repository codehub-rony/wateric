#include <ESP8266WiFi.h>
#include <WebSocketClient.h>
#include "Adafruit_seesaw.h"

boolean handshakeFailed = 0;
//String instructions = "";
char path[] = "/";   //identifier of this device
const char* ssid     = "";
const char* password = "";
String sensor_id = "\"soil_sensor\"";
char* host = "";
const int espport = ;

Adafruit_seesaw ss;
WebSocketClient webSocketClient;

//unsigned long previousMillis = 0;
//unsigned long currentMillis;
//unsigned long interval = 300; //interval for sending data to the websocket server in ms
// Use WiFiClient class to create TCP connections
WiFiClient client;
void setup() {
  Serial.begin(115200);
  delay(10);
  // Connect to wifi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Connect to soil sensor
  if (!ss.begin(0x36)) {
    Serial.println("ERROR! seesaw not found");
    while (1);
  } else {
    Serial.print("seesaw started! version: ");
    Serial.println(ss.getVersion(), HEX);
  }

  delay(1000);

  wsconnect();
}

void loop() {
  if (client.connected()) {
//    webSocketClient.getData(instructions);

    String sensor_measurement = readSensor();
    Serial.println(sensor_measurement);
//    Serial.println(sensor_measurement);

    webSocketClient.sendData(sensor_measurement);//send sensor data to websocket server
  } else {
    Serial.println("connection with websocket lost, trying to reconnect");
    wsconnect();
  }
  delay(5000);
}

void wsconnect() {
  // Connect to the websocket server
  if (client.connect(host, espport)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed.");
    delay(1000);

    if (handshakeFailed) {
      handshakeFailed = 0;
      ESP.restart();
    }
    handshakeFailed = 1;
  }
  // Handshake with the server
  webSocketClient.path = path;
  webSocketClient.host = host;
  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
  } else {

    Serial.println("Handshake failed.");
    delay(4000);

    if (handshakeFailed) {
      handshakeFailed = 0;
      ESP.restart();
    }
    handshakeFailed = 1;
  }
}

// Function for reading soil sensor data
String readSensor() {
  float tempC = ss.getTemp();
  uint16_t capread = ss.touchRead(0);
  String measurement = "{\"sensor_id\": " + sensor_id + ", \"temp\": " + String(tempC) + ", \"moisture\": " + String(capread) + "}";
  return measurement;
}
