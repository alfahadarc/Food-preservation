// servo pin
#include <Servo.h>
#define servoPin 3
#define openPosition 0
#define closePosition 180

Servo myServo;
// motor pin
const int pwm = 5 ;  //initializing pin 2 as pwm
const int in_1 = 8 ;
const int in_2 = 7 ;
int motorVal = 100;
// bme----------------
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

// CCS811------------
#include "Adafruit_CCS811.h"

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
float CO2, Toluene, NH4, Acetone, Temp, P, Hum;
float TVOC;
String data;
int tempThreshHold = 32;
int waterThreshHold = 200;
int hThreshHold = 65;


int waterval = 0; // holds the value
int respin = A0;  // sensor pin used

void MQinit()
{
  MQ135.init();
  MQ135.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ135.setR0(9.03);
}

// MQ135-------------

void setup()
{
  // servo setup
  myServo.attach(servoPin);
  // motor setup
  //pinMode(pwm,OUTPUT) ;   
  pinMode(in_1,OUTPUT) ;  
  pinMode(in_2,OUTPUT) ;
  // put your setup code here, to run once:
  Serial.begin(9600);
  // bme start
  Serial.println(F("BME280 test"));

  bool status;

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin();
  if (!status)
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1)
      ;
  }

  Serial.println("-- Default Test --");

  Serial.println();
  // start mq
  MQinit();

  // start ccs811
  if (!ccs.begin())
  {
    Serial.println("Failed to start ccs811 sensor! Please check your wiring.");
    //while (1)
      ;
  }
  else
  {
    MQCalibration();
  }

  // Wait for the sensor to be ready
  //while (!ccs.available());
 
}

void loop()
{
  // put your main code here, to run repeatedly:
  digitalWrite(in_1,HIGH) ;
  digitalWrite(in_2,LOW) ;


  if (Temp > tempThreshHold || Hum > hThreshHold)
  {//on
    Serial.println(" 250 ");
    analogWrite(pwm,255) ;
    
  }
  else
  {//off
    Serial.println(" 100 ");
    analogWrite(pwm,100) ;
  }

  
  
  bme_call();

  data = data + " M" + String(Temp);
  data = data + " P" + String(P);
  data = data + " H" + String(Hum);

  MQ();
  data = data + " To" + String(Toluene);
  data = data + " N" + String(NH4);
  data = data + " A" + String(Acetone);

  CCS811();
  data = data + " C" + String(CO2);
  data = data + " V" + String(TVOC);

  water();
  data = data + " W" + String(waterval);
  Serial.println(data);
  data = "";
  //motorControl();
  servoControl();

  delay(3000);
}

// MQ function

void MQ()
{
  MQ135.update();
  MQ135.setA(110.47); MQ135.setB(-2.862); //CO2
  CO2 = MQ135.readSensor()*600/5.5;

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
// water level test

void water()
{
  waterval = analogRead(respin); // Read data from analog pin and store it to resval variable
}
// CCS811 function
void CCS811()
{
  if (ccs.available())
  {
    if (!ccs.readData())
    {
     //CO2 = ccs.geteCO2();
      TVOC = ccs.getTVOC();
    }
    else
    {
      Serial.println("ERROR ccs811!");
      // while (1);
    }
  }
}

// bme
void bme_call()
{

  Temp = bme.readTemperature();    //*C
  P = bme.readPressure() / 100.0F; // hPa
  Hum = bme.readHumidity();        // %
}

void printAll()
{
  Serial.print("Toluene:  ");
  Serial.println(Toluene);
  Serial.print("NH4:  ");
  Serial.println(NH4);
  Serial.print("Acetone:  ");
  Serial.println(Acetone);
  Serial.print("CO2:  ");
  Serial.println(CO2);
  Serial.print("TVOC:  ");
  Serial.println(TVOC);
}

void MQCalibration()
{
  Serial.print("Calibrating please wait.");
  float MQ135calcR0 = 0;

  for (int i = 1; i <= 10; i++)
  {
    // Update the voltage lectures
    MQ135.update();
    MQ135calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }

  MQ135.setR0(MQ135calcR0 / 10);
  Serial.println("  done!.");
  Serial.print(MQ135calcR0 / 10);
  Serial.print(" | ");
}

void motorControl()
{
  digitalWrite(pwm,HIGH) ;
  delay(motorVal);
  //digitalWrite(pwm,LOW) ;
  delay(500 - motorVal);

  if (Temp > tempThreshHold)
  {
    Serial.println(" 250 ");
    //analogWrite(pwm, 250);
    motorVal = 255;
  }
  else
  {
    motorVal = 150;
  }
  if (Hum > hThreshHold)
  {
    motorVal = 255;
  }
  else
  {
    motorVal = 150;
  }
  //delay(5000);
}

void servoControl()
{
  if (waterval > waterThreshHold)
  {
    myServo.write(closePosition);
  }
  else
  {
    myServo.write(openPosition);
  }
   
}
