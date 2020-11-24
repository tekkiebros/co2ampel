<div align="center">
    <a href="https://github.com/tekkiebros/co2ampel">
        <img width="150" height="150" src="images/logo.png">
    </a>
    <br>
    <br>
    <h1>CO2 Ampel</h1>
    <p>
        Create your own CO2 Light to fight against Covid-19
    </p>
</div>

## How it works
The project is based on a Wemos D1 mini and a MH-Z19B CO2 sensor.\
The MH-Z19B sensor will give us in a certain interval CO2 gas values and temperature (default every 10 sec).\
One WS2812B Pixel LED will show the air quality based on the values from the [Bundesumweltamt Germany](https://www.umweltbundesamt.de/sites/default/files/medien/pdfs/kohlendioxid_2008.pdf)
- 300-700ppm <span style="color:blue">*Green*</span>
- 700-1000ppm Yellow
- 1000-2000ppm Red
- 2000 and above blinking Red

| Values (ppm) | Color  |
|--------------|--------|
| 300-700 | green | 
| 700-1000| yellow|
| 1000-2000| red|
| >2000|blinking red|

## What you need
- Wemos D1 Mini (may work with other ESP8266 Devices too)
- MH-Z19B CO2 Sensor
- WS2812B LED
- Jumper Wires

## How to install
Clone this repository to your computer or copy the code for the [Simple Version](https://github.com/tekkiebros/co2ampel/tree/main/CO2_ampel_simple) or [Access Point Version](https://github.com/tekkiebros/co2ampel/tree/main/CO2_ampel_AP)
