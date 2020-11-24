#include <Adafruit_NeoPixel.h>
#include "MHZ19.h"
#include <SoftwareSerial.h>


#define CO2_TX 5 //D1
#define CO2_RX 4 //D2

#define LED_PIN 14 //D5
#define LED_COUNT 1

int coWert;
int temp;

MHZ19 myMHZ19;    

SoftwareSerial SerialCO2(CO2_RX, CO2_TX); 
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
uint32_t green = strip.Color(0, 255, 0);
uint32_t yellow = strip.Color(255,140,0);
uint32_t red = strip.Color(255, 0, 0);
uint32_t blue = strip.Color(0, 0, 255);


void setup() {
  Serial.begin(9600);
  
  SerialCO2.begin(9600);
  myMHZ19.begin(SerialCO2);
  strip.begin();
  strip.show();
  
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
}


void getValues(){
  coWert = myMHZ19.getCO2();
  Serial.print("CO2 (ppm): ");
  Serial.println(coWert);
  temp = myMHZ19.getTemperature();                     // Request Temperature (as Celsius)
  Serial.print("Temperature (C): ");                  
  Serial.println(temp); 
}

void loop() {
  getValues();
  strip.clear();
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
  delay(5000); 
}
