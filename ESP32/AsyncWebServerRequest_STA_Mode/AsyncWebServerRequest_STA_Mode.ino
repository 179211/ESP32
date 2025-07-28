/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-client-server-wi-fi/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
// https://github.com/me-no-dev/ESPAsyncWebServer
// go to Sketch > Include Library > Add .zip Library and select the libraries you’ve just downloaded

// Set your access point network credentials
const char* ssid = "_Naruto_";  // Enter SSID here
const char* password = "WakeUpNow@5";  //Enter Password here

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

uint8_t LED1pin =2;
bool LED1status = HIGH;

uint8_t LED2pin = 5;
bool LED2status = LOW;

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  delay(100);
  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);
  Serial.println();
  
  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP()); 
  Serial.print("Got MAC: ");  Serial.println(WiFi.macAddress());
  Serial.println("To list connected devices: cmd > arp -a");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", handle_OnConnect().c_str());
  });
  server.on("/led1on", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", handle_led1on().c_str());
  });
  server.on("/led1off", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", handle_led1off().c_str());
  });
  server.on("/led2on", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", handle_led2on().c_str());
  });
  server.on("/led2off", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", handle_led2off().c_str());
  });
  server.onNotFound(handle_NotFound);
    
  // Start server
  server.begin();
}
 
void loop(){  
}

void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  Serial.println("GPIO4 Status: OFF | GPIO5 Status: OFF");
  return SendHTML(LED1status,LED2status); 
}

void handle_led1on() {
  LED1status = HIGH;
  Serial.println("GPIO4 Status: ON");
  return SendHTML(true,LED2status); 
}

void handle_led1off() {
  LED1status = LOW;
  Serial.println("GPIO4 Status: OFF");
  return SendHTML(false,LED2status); 
}

void handle_led2on() {
  LED2status = HIGH;
  Serial.println("GPIO5 Status: ON");
  return SendHTML(LED1status,true); 
}

void handle_led2off() {
  LED2status = LOW;
  Serial.println("GPIO5 Status: OFF");
  return SendHTML(LED1status,false); 
}

void handle_NotFound(){
  return  "Not found" ;
}

String SendHTML(uint8_t led1stat,uint8_t led2stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP32 Web Server</h1>\n";
    ptr +="<h3>Using Station(STA) Mode</h3>\n";
  
   if(led1stat)
  {ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";}
  else
  {ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";}

  if(led2stat)
  {ptr +="<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";}
  else
  {ptr +="<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
