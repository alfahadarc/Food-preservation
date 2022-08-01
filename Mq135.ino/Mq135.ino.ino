#include <Wire.h> 
#include <MQUnifiedsensor.h>
#define         Board                   ("Arduino UNO")
#define         Pin135                   (A2)  //Analog input 2 of your arduino
#define         RatioMQ135CleanAir        (3.6) //RS / R0 = 10 ppm 
#define         ADC_Bit_Resolution        (10) // 10 bit ADC 
#define         Voltage_Resolution        (5) // Volt resolution to calc the voltage
#define         Type                      ("Arduino UNO") //Board used
//Declare Sensor
MQUnifiedsensor MQ135(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin135, Type);


void setup() {
  Serial.begin(9600);
  MQinit();
}

void MQinit(){
  MQ135.init();
  MQ135.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ135.setR0(9.03);
}
void loop() {
  
  MQ135.update();
  MQ135.setA(110.47); MQ135.setB(-2.862); //CO2 
float CO2 = MQ135.readSensor(); 
  
  MQ135.setA(44.947); MQ135.setB(-3.445); // Toluene
float Toluene = MQ135.readSensor(); 
  
  MQ135.setA(102.2 ); MQ135.setB(-2.473); //NH4 
float NH4 = MQ135.readSensor(); 
  
  MQ135.setA(34.668); MQ135.setB(-3.369); //Acetone
float Acetone = MQ135.readSensor();

  Serial.print("CO2:      "); Serial.println(CO2);
  Serial.print("Toluene:  "); Serial.println(Toluene);
  Serial.print("NH4:      "); Serial.println(NH4);
  Serial.print("Acetone:  "); Serial.println(Acetone);
  Serial.println("--------------------------------------------------------");
  delay(2000);
}
