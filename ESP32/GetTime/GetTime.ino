#include <WiFi.h>
#include "time.h"

const char* ssid       = "Airtel_tush_9922";
const char* password   = "air33477";

const char* ntpServer = "asia.pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void setup()
{
  Serial.begin(115200);
  
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP()); 
  Serial.print("Got MAC: ");  Serial.println(WiFi.macAddress());
  Serial.println("To list connected devices: cmd > arp -a");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  //WiFi.disconnect(true);
  //WiFi.mode(WIFI_OFF);
}

void loop()
{
  delay(2000);
  //configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
}
