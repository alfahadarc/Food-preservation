// CCS811------------
#include "Adafruit_CCS811.h"
#include <Servo.h>

Adafruit_CCS811 ccs;
// CCS811------------
// MQ135-------------
#include <Wire.h>
#include <MQUnifiedsensor.h>
#define Board ("Arduino UNO")
#define Pin135 (A2)              // Analog input 2 of your arduino
#define RatioMQ135CleanAir (3.6) // RS / R0 = 10 ppm
#define ADC_Bit_Resolution (10)  // 10 bit ADC
#define Voltage_Resolution (5)   // Volt resolution to calc the voltage
#define Type ("Arduino UNO")     // Board used
// Declare Sensor
MQUnifiedsensor MQ135(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin135, Type);

// global variables
float CO2, Toluene, NH4, Acetone;
float TVOC;
String data;

Servo myservo;

int resval = 0;  // holds the value
int respin = A0; // sensor pin used

int pos = 0;
  
void MQinit()
{
  MQ135.init();
  MQ135.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ135.setR0(9.03);
}

// MQ135-------------

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);  
  // start mq
  MQinit();

  // start ccs811
  if (!ccs.begin())
  {
    Serial.println("Failed to start sensor! Please check your wiring.");
    while (1);
  }else{
    MQCalibration();
  }

  // Wait for the sensor to be ready
  while (!ccs.available());

  myservo.attach(9);

}

void loop()
{
  // put your main code here, to run repeatedly:
  
  MQ();
  data = data + " To" + String(Toluene);
  data = data + " N" + String(NH4);
  data = data + " A" + String(Acetone);
  
  CCS811();
  data = data + " C" + String(CO2);
  data = data + " V" + String(TVOC);

  water();
  data = data + " W" + String(resval);
  Serial.println(data);

  //servo();
  
  

  delay(4000);
  data = "";
}

//water level test

void water() { 
  resval = analogRead(respin); //Read data from analog pin and store it to resval variable  
}

void servo() {
  int a = myservo.read();
    //Serial.println(String(a));
  if(resval>300){
    if(myservo.read()>=-10 && myservo.read()<10){
      for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(10);                       // waits 15 ms for the servo to reach the position
      }
    }
  }else{    
    if(myservo.read()<=100 && myservo.read()>80){
     for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(10);                       // waits 15 ms for the servo to reach the position
      }
    }
  }
}


// MQ function

void MQ()
{
  MQ135.update();
  // MQ135.setA(110.47); MQ135.setB(-2.862); //CO2
  // CO2 = MQ135.readSensor();

  MQ135.setA(44.947);
  MQ135.setB(-3.445); // Toluene
  Toluene = MQ135.readSensor();

  MQ135.setA(102.2);
  MQ135.setB(-2.473); // NH4
  NH4 = MQ135.readSensor();

  MQ135.setA(34.668);
  MQ135.setB(-3.369); // Acetone
  Acetone = MQ135.readSensor();
}

// CCS811 function
void CCS811()
{
  if (ccs.available())
  {
    if (!ccs.readData())
    {
      CO2 = ccs.geteCO2();
      TVOC = ccs.getTVOC();
    }
    else
    {
      Serial.println("ERROR!");
     // while (1);
    }
  }
}

void printAll(){
  Serial.print("Toluene:  ");
  Serial.println(Toluene);
  Serial.print("NH4:  ");n/ 
  Serial.println(NH4);
  Serial.print("Acetone:  ");
  Serial.println(Acetone);
  Serial.print("CO2:  ");
  Serial.println(CO2);
  Serial.print("TVOC:  ");
  Serial.println(TVOC);
  Serial.print("Water Level: ");
  Serial.println(resval);
}

void MQCalibration(){
  Serial.print("Calibrating please wait.");
  float MQ135calcR0 = 0;
         
  for (int i = 1; i <= 10; i ++)
  {
    //Update the voltage lectures
    MQ135.update();
    MQ135calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }

  MQ135.setR0(MQ135calcR0 / 10);
  Serial.println("  done!.");
  Serial.print(MQ135calcR0 / 10); Serial.print(" | ");
}
