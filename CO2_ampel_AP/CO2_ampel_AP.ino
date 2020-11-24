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

/* Function for HTTP Request / */
void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(coWert, temp)); 
}

/* Function for HTTP Error */
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

/* HTML Content for Webpage */
String SendHTML(int ppm, int temp){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta charset=\"utf-8\" name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>CO2 Ampel</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".dotGreen{height: 100px;width: 100px;background-color: green;border-radius: 50%; display: inline-block;}\n";
  ptr +=".dotYellow{height: 100px;width: 100px;background-color: yellow;border-radius: 50%; display: inline-block;}\n";
  ptr +=".dotRed{height: 100px;width: 100px;background-color: red;border-radius: 50%; display: inline-block;}\n";
  ptr +="p {font-size: 18px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Corona CO2 Ampel</h1>\n";
  ptr +="<p>Temperatur aktuell: " + String(temp) +"°C</p>\n";
  ptr +="<p>CO2 Wert aktuell: " + String(ppm) +"ppm</p>\n";
  if(ppm>=300 && ppm<700){
    ptr +="<br/><span class=\"dotGreen\"></span>\n";
  }
  if(ppm>=700 && ppm<1000){
    ptr +="<br/><span class=\"dotYellow\"></span>\n";
  }
  if(ppm>=1000){
    ptr +="<br/><span class=\"dotRed\"></span>\n";
  }
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

/*Set Colors */
void setColor(int coWert){
  if(coWert>=300 && coWert<700){
    strip.setPixelColor(0, green);
    strip.show();
  }
  if(coWert>=700 && coWert<1000){
    strip.setPixelColor(0, yellow);
    strip.show();
  }
  if(coWert>=1000 && coWert<2000){
    strip.setPixelColor(0, red);
    strip.show();
  }
  if(coWert>=2000){
    for(int k=0;k<=5; k++){
      strip.setPixelColor(0, red);
      strip.show();
      delay(500);
      strip.setPixelColor(0,0,0,0);
      strip.show();
      delay(500);
    }
    return;
  }

}

/* Get CO2 and Temperature from Sensor */
void getValues(){
  coWert = myMHZ19.getCO2();
  Serial.print("CO2 (ppm): ");
  Serial.println(coWert);
  temp = myMHZ19.getTemperature();
  Serial.print("Temperature (C): ");                  
  Serial.println(temp); 
}

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
