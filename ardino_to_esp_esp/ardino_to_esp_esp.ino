// this sample code provided by www.programmingboss.com
#define RXp2 16
#define TXp2 17
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
}
void loop() {
    Serial.println("Message Received: ");
    String tmp = Serial2.readString(); 
    Serial.println(tmp);
    float temp = tmp.toFloat();
    if(temp < 25.0){
      Serial.print("less: " );
      Serial.println(temp);
    }else{
      Serial.print("high: " );
      Serial.println(temp);
    }
    
}
