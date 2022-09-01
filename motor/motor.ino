const int pwm = 5 ;  //initializing pin 2 as pwm
const int in_1 = 8 ;
const int in_2 = 7 ;

//For providing logic to L298 IC to choose the direction of the DC motor 

void setup()
{
pinMode(pwm,OUTPUT) ;   
pinMode(in_1,OUTPUT) ;  
pinMode(in_2,OUTPUT) ;
}

void loop()
{
//For Clock wise motion , in_1 = High , in_2 = Low

digitalWrite(in_1,HIGH) ;
digitalWrite(in_2,LOW) ;
analogWrite(pwm,255) ;
delay(5000);
analogWrite(pwm,100) ;
delay(5000);
 }
