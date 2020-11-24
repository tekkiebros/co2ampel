/* Get CO2 and Temperature from Sensor */
void getValues(){
  coWert = myMHZ19.getCO2();
  Serial.print("CO2 (ppm): ");
  Serial.println(coWert);
  temp = myMHZ19.getTemperature();
  Serial.print("Temperature (C): ");                  
  Serial.println(temp); 
}
