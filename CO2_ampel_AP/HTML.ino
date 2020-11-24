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
