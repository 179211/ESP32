#include <MyWifi.h>

#include <WebServer.h>
#include <HTTPClient.h>
#include "time.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>

/// Pin configuration
// Touch 4, 12, 13, 14, 15, 27, 32, 33
// Touch 12, 14, 15
#define OnBoardLed 2
#define pinBtn1 26
#define pinBtn2 25
#define pinBtn3 33
#define pinBtn4 32
#define pinBtn5 35
#define pinBtn6 34
#define pinBtn7 39
#define pinBtn8 36 // On board button

#define pinRelay1 4
#define pinRelay2 16
#define pinRelay3 17
#define pinRelay4 18
#define pinRelay5 19
#define pinRelay6 23 //LED
#define pinRelay7 27 //LED OR BUZZER
#define pinRelay8 13

/// Display & Wifi configuration
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
extern Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
extern MyDisplay myDisplay(true);
MyWifi myWifi(OnBoardLed); // On board led


/// Time Server configuration
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;
unsigned long now = millis();
struct tm timeinfo;
int currentDate = 0;

/// Web Server configuration
WebServer server(80);

/// Custom configuration
String msg = "";

// Button configuration
struct Button {
  const uint8_t PIN;
  String Name;
  const uint8_t PIN_Relay;
  uint32_t lastTrigger;
  uint32_t timeSeconds;
  boolean startTimer;
  boolean processButton;
};
void buttonPressed(Button btn){
  if (millis() - btn.lastTrigger > 500) {
    btn.lastTrigger = millis();
    msg = "Button Pressed: "+btn.Name;
    Serial.println(msg);
    btn.processButton = true;
    myDisplay.showMsg(msg);
  }
}

// Creating Toggle Button
Button Btn8 = {pinBtn8, "FAN", pinRelay8, 0, 0, false, false};
void IRAM_ATTR buttonIntp8() {
  buttonPressed(Btn8);
}


void setup() {
  if(!getLocalTime(&timeinfo)){
    initTime();
  }

  pinMode(Btn8.PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(Btn8.PIN), buttonIntp8, FALLING);
}


void loop() {
  // Current time
  now = millis();
  checkWifi();
  updateDisplay();

  if (Btn8.processButton){
    Btn8.processButton = false;
    if (digitalRead(Btn8.PIN_Relay) == LOW)
      digitalWrite(Btn8.PIN_Relay, HIGH);
    else
      digitalWrite(Btn8.PIN_Relay, LOW);
  } 

}


void initTime(){
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  updateDisplay();
}

void updateDisplay(){
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  //Serial.println(&timeinfo, "%d %b %H:%M:%S %a");

  display.clearDisplay();
  display.setCursor(0,0);
  display.println(&timeinfo, "%d %b %H:%M:%S %a");
  display.print("IP: "); display.println(myWifi.IP.toString().c_str());
  display.display();
}

void checkWifi(){
  if (WiFi.status() != WL_CONNECTED ) //&& timeinfo.tm_hour != myWifi.wifiLastRetry)
  {
    myWifi.wifiLastRetry = millis();
    myWifi.initWifi(false);
    digitalWrite(OnBoardLed, HIGH);
  }
  else{
    digitalWrite(OnBoardLed, LOW);
  }

}
