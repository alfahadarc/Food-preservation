// wifi
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <Wire.h>

//for mail
#include <Arduino.h>

#include <ESP_Mail_Client.h>

#define WIFI_SSID "Aronnok"
#define WIFI_PASSWORD "337077dark_world"

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

/* The sign in credentials */
#define AUTHOR_EMAIL "esp32mail316@gmail.com"
#define AUTHOR_PASSWORD "lqbsjsrrmcaaslub"

/* Recipient's email*/
#define RECIPIENT_EMAIL "alfahadarc@gmail.com"

/* The SMTP Session object used for Email sending */
SMTPSession smtp;

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

// comm from arduino
#define RXp2 16
#define TXp2 17
// comm from arduino

// globals
String CO2, Toluene, NH4, Acetone, TVOC, WL, Temp, P, Hum;

float waterThreshHold = 200;
float CO2th = 1000;
float NH4th = 15;
float TempthHigh = 32;
float TempthLow = 15;
float HumthHigh = 65;
float HumthLow = 30;

//int msgFlag = 0;
int waterMsgFlag = 0;
int co2MsgFlag = 0;
int nh4MsgFlag = 0;
int tMsgFlag = 0;
int hMsgFlag = 0;

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
  //mail setup
  
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
  sendMessgae();
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

void smtpCallback(SMTP_Status status){
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success()){
    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
    Serial.println("----------------\n");
    struct tm dt;

    for (size_t i = 0; i < smtp.sendingResult.size(); i++){
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;
      localtime_r(&ts, &dt);

      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients);
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject);
    }
    Serial.println("----------------\n");
  }
}

String getCO2Value(){
  if(CO2.toFloat()>CO2th){
    return "Warning: CO2 amount is high (" + CO2 + "ppm)\n";
  }
  return "CO2 amount is optimal (" + CO2 + "ppm)\n";
}


String getWLValue(){
  if(WL.toFloat()>waterThreshHold){
    return "Warning: Water level is high\n";
  }
  return "Water level is optimal\n";
}

String getTolValue(){
  return "Toluene level is " + Toluene + "ppm\n";
}


String getNH4Value(){
  if(NH4.toFloat()>NH4th){
    return "Warning: NH4 level is high (" + NH4 + "ppm)\n";
  }
  return "NH4 level is optimal (" + NH4 + "ppm)\n";
}


String getAcetoneValue(){
  return "Acetone level is " + Acetone + "ppm\n";
}

String getTVOCValue(){
  return "TVOC level is " + TVOC + "ppm\n";
}

String getTempValue(){
  if(Temp.toFloat()>TempthHigh){
    return "Warning: Temperature level is high (" + Temp + "C)\n";
  }else if(Temp.toFloat()<TempthLow){
    return "Warning: Temperature level is low (" + Temp + "C)\n";
  }
  return "Temperature level is optimal (" + Temp + "C)\n";
}


String getPValue(){
  return "Air Pressure is " + P + "hPa\n";
}

String getHumValue(){
  if(Hum.toFloat()>HumthHigh){
    return "Warning: Humidity level is high (" + Hum + "%)\n";
  }else if(Hum.toFloat()<HumthLow){
    return "Warning: Humidity level is low (" + Hum + "%)\n";
  }
  return "Humidity level is optimal (" + Hum + "%)\n";
}


void sendMessgae(){
  if( (waterMsgFlag == 0) && (WL.toFloat() > waterThreshHold)){
    String msg =  getWLValue()+getTempValue()+getHumValue()+getCO2Value()+getTolValue()+getNH4Value()+getAcetoneValue()+getTVOCValue()+getPValue();    
    sendMsg(msg);
    waterMsgFlag = 1;
  }else if( (co2MsgFlag == 0) && (CO2.toFloat()>CO2th)){
    String msg =  getWLValue()+getTempValue()+getHumValue()+getCO2Value()+getTolValue()+getNH4Value()+getAcetoneValue()+getTVOCValue()+getPValue();    
    sendMsg(msg);
    co2MsgFlag = 1;
  }else if( (nh4MsgFlag == 0) && (NH4.toFloat()>NH4th)){
    String msg =  getWLValue()+getTempValue()+getHumValue()+getCO2Value()+getTolValue()+getNH4Value()+getAcetoneValue()+getTVOCValue()+getPValue();    
    sendMsg(msg);
    nh4MsgFlag = 1;
  }else if( (tMsgFlag == 0) && (Temp.toFloat()>TempthHigh || Temp.toFloat()<TempthLow)){
    String msg =  getWLValue()+getTempValue()+getHumValue()+getCO2Value()+getTolValue()+getNH4Value()+getAcetoneValue()+getTVOCValue()+getPValue();    
    sendMsg(msg);
    tMsgFlag = 1;
  }else if( ( hMsgFlag == 0) && (Hum.toFloat()>HumthHigh || Hum.toFloat()<HumthLow)){
    String msg =  getWLValue()+getTempValue()+getHumValue()+getCO2Value()+getTolValue()+getNH4Value()+getAcetoneValue()+getTVOCValue()+getPValue();    
    sendMsg(msg);
     hMsgFlag = 1;
  }
  if(waterMsgFlag == 1 && WL.toFloat() < waterThreshHold) {
    waterMsgFlag = 0;
  }else if((co2MsgFlag == 1) && (CO2.toFloat()<CO2th)){
    co2MsgFlag == 0;
  }else if((nh4MsgFlag == 1) && (NH4.toFloat()<NH4th)){
    nh4MsgFlag == 0;
  }else if((tMsgFlag == 1) && (Temp.toFloat()<TempthHigh && Temp.toFloat()>TempthLow)){
    tMsgFlag == 0;
  }else if(( hMsgFlag == 1) && (Hum.toFloat()<HumthHigh && Hum.toFloat()>HumthLow)){
    hMsgFlag == 0;
  }
}
void sendMsg(String msg){
  smtp.debug(1);

  /* Set the callback function to get the sending results */
  smtp.callback(smtpCallback);

  /* Declare the session config data */
  ESP_Mail_Session session;

  /* Set the session config */
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";

  /* Declare the message class */
  SMTP_Message message;

  /* Set the message headers */
  message.sender.name = "ESP";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "ESP Test Email";
  message.addRecipient("Fahad", RECIPIENT_EMAIL);



  
  //Send raw text message
  String textMsg = "Caution!\n"+msg;
  message.text.content = textMsg.c_str();
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

  /* Set the custom message header */
  //message.addHeader("Message-ID: <abcde.fghij@gmail.com>");

  /* Connect to server with the session config */
  if (!smtp.connect(&session))
    return;

  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error sending Email, " + smtp.errorReason());
}
