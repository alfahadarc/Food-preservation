// this sample code provided by www.programmingboss.com
#define RXp2 16
#define TXp2 17


//globals
float CO2, Toluene, NH4, Acetone;
float TVOC;
//string setup
String strs[20];
int StringCount = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
}
void loop() {
    Serial.println("Message Received: ");
    String data = Serial2.readString(); 
    Serial.println(data);
    Serial.println(".....");
    string_split(data);
    delay(4000);
    
}


void string_split(String str){
  while (str.length() > 0)
  {
    int index = str.indexOf(' ');
    if (index == -1) // No space found
    {
      Serial.println(str);
      break;
    }
    else
    {
      String val;
      val = str.substring(0, index);
      str = str.substring(index+1);
      Serial.println(val);
    }
  }
}
