
/*
  #######################################
  #                                     #
  #           Code by TekkieBros        #
  #                                     #
  #             Visit us on             #
  #       https://www.tekkiebros.de     #
  #                                     #
  #######################################
*/
#include "credentials.h"
#include <Adafruit_NeoPixel.h>
#include "MHZ19.h"
#include <SoftwareSerial.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

#define CO2_TX 5 //D1
#define CO2_RX 4 //D2

#define LED_PIN 14 //D5
#define LED_COUNT 1

int coWert;
int temp;

long previousMillis = 0;
long interval = 60000;

// Wifi
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWD;
// MQTT
const char* mqtt_server = MQTT_SERVER;
const char* mqtt_user = MQTT_USER;
const char* mqtt_password = MQTT_PASSWD;
const char* mqtt_clientId = "Corona Ampel";
const char* outTopicMsgCoWert = "tekkiebros/coronaAmpel/coWert";
const char* outTopicMsgTemp = "tekkiebros/coronaAmpel/temp";


MHZ19 myMHZ19;

SoftwareSerial SerialCO2(CO2_RX, CO2_TX);
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
uint32_t green = strip.Color(0, 255, 0);
uint32_t yellow = strip.Color(255, 140, 0);
uint32_t red = strip.Color(255, 0, 0);
uint32_t blue = strip.Color(0, 0, 255);

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);

  Serial.println(ssid);
  // Setup Wifi
  setup_wifi();

  // Setup MQTT
  client.setServer(mqtt_server, 1883);

  SerialCO2.begin(9600);
  myMHZ19.begin(SerialCO2);
  strip.begin();
  strip.show();
/*
  int i;
    Serial.println("Aufheitzen");
    while(millis()<180000){
    Serial.print(".");

    for(i=0; i<=255; i++){
      strip.setPixelColor(0, strip.Color(i, 0, 0));
      strip.show();
      delay(10);
    }
      for(i=255; i>=0; i--){
      strip.setPixelColor(0, strip.Color(i, 0, 0));
      strip.show();
      delay(10);
    }
    }
  Serial.println("Ready!");
  */
}


void setup_wifi() {
  int wifiCounter = 0;
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    if (wifiCounter < 10) {
      delay(500);
      Serial.print(".");
    } else {
      Serial.println("Failed to connect to Wifi!");
      wifiCounter = 0;
    }
    wifiCounter++;
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Client ID connected
    if (client.connect(mqtt_clientId, mqtt_user, mqtt_password)) {
      Serial.print(mqtt_clientId);
      Serial.println(" connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void getValues() {
  coWert = myMHZ19.getCO2();
  Serial.print("CO2 (ppm): ");
  Serial.println(coWert);
  temp = myMHZ19.getTemperature();                     // Request Temperature (as Celsius)
  Serial.print("Temperature (C): ");
  Serial.println(temp);
}

void loop() {
  getValues();

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    client.publish(outTopicMsgCoWert, String(coWert).c_str());
    client.publish(outTopicMsgTemp, String(temp).c_str());
  }

  strip.clear();
  if (coWert >= 300 && coWert < 700) {
    strip.setPixelColor(0, green);
    strip.show();
  }
  if (coWert >= 700 && coWert < 1000) {
    strip.setPixelColor(0, yellow);
    strip.show();
  }
  if (coWert >= 1000 && coWert < 2000) {
    strip.setPixelColor(0, red);
    strip.show();
  }
  if (coWert >= 2000) {
    for (int k = 0; k <= 5; k++) {
      strip.setPixelColor(0, red);
      strip.show();
      delay(500);
      strip.setPixelColor(0, 0, 0, 0);
      strip.show();
      delay(500);
    }
    return;
  }
  delay(5000);
}
