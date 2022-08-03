//wifi
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <Wire.h>



//comm from arduino
#define RXp2 16
#define TXp2 17
//comm from arduino

//globals
String CO2, Toluene, NH4, Acetone, TVOC;

//server
// Replace with your network credentials
const char* ssid = "Aronnok";
const char* password = "337077dark_world";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readings_CO2() {
   return CO2;
}

String readings_Toluene(){
  return Toluene;
}

String readings_NH4(){
  return NH4;
}
String readings_Acetone(){
  return Acetone;
}
String readings_TVOC(){
  return TVOC;
}
void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);

  // Initialize SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

   // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html");
  });
  server.on("/readings_CO2", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", readings_CO2().c_str());
  });
  server.on("/readings_Toluene", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", readings_Toluene().c_str());
  });
  server.on("/readings_NH4", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", readings_NH4().c_str());
  });
  server.on("/readings_Acetone", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", readings_Acetone().c_str());
  });
  server.on("/readings_TVOC", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", readings_TVOC().c_str());
  });
  // Start server
  server.begin();

}

void loop() {
  Serial.println("Message Received: ");
  String data = Serial2.readString();
  Serial.println(data);
  Serial.println(".....");
  string_split(data);
  delay(4000);

}


void string_split(String str) {
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
      str = str.substring(index + 1);
      if(val.indexOf("C") > -1){
        int v = val.indexOf("C");
        CO2 = val.substring(v+1);
      }
      if(val.indexOf("N") > -1){
        int v = val.indexOf("N");
        NH4 = val.substring(v+1);
      }
      if(val.indexOf("To") > -1){
        int v = val.indexOf("To");
        Toluene = val.substring(v+2);
      }
      if(val.indexOf("A") > -1){
        int v = val.indexOf("A");
        Acetone = val.substring(v+1);
      }
      if(val.indexOf("V") > -1){
        int v = val.indexOf("V");
        TVOC = val.substring(v+1);
      }
      Serial.println(val);
    }
  }
}
