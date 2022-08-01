  #include <WiFi.h>
  #include <Wire.h>
  #include <ESPAsyncWebServer.h>
  #include <SPIFFS.h>

  //sensors 
  #include <Adafruit_Sensor.h>  // include Adafruit sensor library
  #include <Adafruit_BMP280.h>  // include adafruit library for BMP280 sensor

  #define BMP280_I2C_ADDRESS  0x76

  Adafruit_BMP280 bmp280;



  //arduino to esp32 serial
  #define RXp2 16
  #define TXp2 17

// Replace with your network credentials
#define WIFI_SSID "Aronnok"
#define WIFI_PASSWORD "337077dark_world"

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readBME280Temperature() {
  // Read temperature as Celsius (the default)
    float temperature = bmp280.readTemperature();  // get temperature
    return String(temperature);
}


void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);

  //initialize bmp
    if (!bmp280.begin(BMP280_I2C_ADDRESS))
  {  
    //Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
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
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readBME280Temperature().c_str());
  });
  // Start server
  server.begin();
}
 
void loop(){
  
}
