

#include <Wire.h>             // include Wire library, required for I2C devices
#include <Adafruit_Sensor.h>  // include Adafruit sensor library
#include <Adafruit_BMP280.h>  // include adafruit library for BMP280 sensor


// define device I2C address: 0x76 or 0x77 (0x77 is library default address)
#define BMP280_I2C_ADDRESS  0x76

Adafruit_BMP280 bmp280;


void setup() {
  Serial.begin(9600);

 
  
  //Serial.println(F("Arduino + BMP280"));
  
  if (!bmp280.begin(BMP280_I2C_ADDRESS))
  {  
    //Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }

  
}



// main loop
void loop()
{
  // get temperature, pressure and altitude from library
  float temperature = bmp280.readTemperature();  // get temperature
  float pressure    = bmp280.readPressure();     // get pressure
  float altitude_   = bmp280.readAltitude(1013.25); // get altitude (this should be adjusted to your local forecast)

  

  // print data on the serial monitor software
  // 1: print temperature
  //Serial.print("Temperature = ");
  Serial.print(temperature);
  //Serial.println(" °C");

    
  Serial.println();  // start a new line
  delay(2000);       // wait 2 seconds
  
}
// end of code.
