// wifi
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <Wire.h>

// comm from arduino
#define RXp2 16
#define TXp2 17
// comm from arduino

// globals
String CO2, Toluene, NH4, Acetone, TVOC, WL, Temp, P, Hum;

// server
//  Replace with your network credentials
const char *ssid = "POCO";
const char *password = "";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readings_CO2()
{
  return CO2;
}

String readings_Toluene()
{
  return Toluene;
}

String readings_NH4()
{
  return NH4;
}
String readings_Acetone()
{
  return Acetone;
}
String readings_TVOC()
{
  return TVOC;
}
String readings_WL()
{
  return WL;
}

//-----------------------need to modify
String readBME280Temperature()
{
  return Temp;
}
String readBME280Humidity()
{
  return Hum;
}
String readBME280Pressure()
{
  return P;
}
//-----------------------need to modify
void setup()
{
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);

  // Initialize SPIFFS
  if (!SPIFFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html"); });
  server.on("/readings_CO2", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", readings_CO2().c_str()); });
  server.on("/readings_Toluene", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", readings_Toluene().c_str()); });
  server.on("/readings_NH4", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", readings_NH4().c_str()); });
  server.on("/readings_Acetone", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", readings_Acetone().c_str()); });
  server.on("/readings_TVOC", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", readings_TVOC().c_str()); });
  server.on("/readings_WL", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", readings_WL().c_str()); });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", readBME280Temperature().c_str()); });

  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", readBME280Humidity().c_str()); });

  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", readBME280Pressure().c_str()); });
  // Start server
  server.begin();
}

void loop()
{
  Serial.println("Message Received: ");
  String data = Serial2.readString();
  // Serial.println(data);
  // Serial.println(".....");
  string_split(data);
  delay(3000);
}

void string_split(String str)
{
  while (str.length() > 0)
  {
    int index = str.indexOf(' ');
    if (index == -1) // No space found
    {
      String val;
      val = str;
      if (val.indexOf("W") > -1)
      {
        int v = val.indexOf("W");
        WL = val.substring(v + 1);
        Serial.print(" W ");
        Serial.println(WL);
      }
      break;
    }
    else
    {
      String val;
      val = str.substring(0, index);
      str = str.substring(index + 1);
      if (val.indexOf("M") > -1)
      {
        int v = val.indexOf("M");
        Temp = val.substring(v + 1);
        Serial.print(" M ");
        Serial.println(Temp);
      }
      if (val.indexOf("P") > -1)
      {
        int v = val.indexOf("P");
        P = val.substring(v + 1);
        Serial.print(" P ");
        Serial.println(P);
      }
      if (val.indexOf("H") > -1)
      {
        int v = val.indexOf("H");
        Hum = val.substring(v + 1);

        Serial.print(" H ");
        Serial.println(Hum);
      }
      if (val.indexOf("C") > -1)
      {
        int v = val.indexOf("C");
        CO2 = val.substring(v + 1);

        Serial.print(" C ");
        Serial.println(CO2);
      }
      if (val.indexOf("N") > -1)
      {
        int v = val.indexOf("N");
        NH4 = val.substring(v + 1);

        Serial.print(" N ");
        Serial.println(NH4);
      }
      if (val.indexOf("To") > -1)
      {
        int v = val.indexOf("To");
        Toluene = val.substring(v + 2);

        Serial.print(" To ");
        Serial.println(Toluene);
      }
      if (val.indexOf("A") > -1)
      {
        int v = val.indexOf("A");
        Acetone = val.substring(v + 1);

        Serial.print(" A ");
        Serial.println(Acetone);
      }
      if (val.indexOf("V") > -1)
      {
        int v = val.indexOf("V");
        TVOC = val.substring(v + 1);

        Serial.print(" V ");
        Serial.println(TVOC);
      }
      if (val.indexOf("W") > -1)
      {
        int v = val.indexOf("W");
        WL = val.substring(v + 2);
      }
    }
  }
}
