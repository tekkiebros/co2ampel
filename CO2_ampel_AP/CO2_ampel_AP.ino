#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include "MHZ19.h"
#include <SoftwareSerial.h>

/* Define Pins for MH-Z19B Sensor */
#define CO2_TX 5 //D1
#define CO2_RX 4 //D2

/* Define Pin for WS2812B LED */
#define LED_PIN 14 //D5
#define LED_COUNT 1

/* Put your SSID & Password */
const char* ssid = "CO2Ampel";  // Enter SSID here
const char* password = "Covid-19";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

/*Declare Variables */
int coWert;
int temp;
bool heating = true;

/* Create Object for MH-Z19B Sensor */
MHZ19 myMHZ19;

/* declare an object of ESP8266WebServer library */
ESP8266WebServer server(80);  

/* Start Serial Connection to MH-Z19B Sensor */
SoftwareSerial SerialCO2(CO2_RX, CO2_TX);

/* Create LED Object and define Colors */
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
uint32_t green = strip.Color(0, 255, 0);
uint32_t yellow = strip.Color(255,140,0);
uint32_t red = strip.Color(255, 0, 0);
uint32_t blue = strip.Color(0, 0, 255);

/* Setup */
void setup() {
  Serial.begin(9600);

  /* Create Access Point */
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  /* Configure HTTP Server */
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");

  /* Start Serial */
  SerialCO2.begin(9600);
  myMHZ19.begin(SerialCO2);

  /* Start LED */
  strip.begin();
  strip.show();

  /* If heating true, MH-Z19B will heating 3 Minutes */
  if(heating){
    Serial.println("Aufheitzen");
    while(millis()<180000){
      Serial.print(".");

      /* Pulse LED red while heating */
      for(int i=0; i<=255; i++){
        strip.setPixelColor(0, strip.Color(i, 0, 0));
        strip.show();
        delay(10);
      }
        for(int i=255; i>=0; i--){
        strip.setPixelColor(0, strip.Color(i, 0, 0));
        strip.show();
        delay(10);
      }
    } 
  }
 
  Serial.println("Ready!");
}

/* Loop */
void loop() {  
  getValues();
  server.handleClient();
  
  strip.clear();
  
  setColor(coWert);
  
  delay(5000); 
}
