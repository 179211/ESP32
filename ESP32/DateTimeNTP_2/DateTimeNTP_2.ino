/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-date-time-ntp-client-server-arduino/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <WiFi.h>

#include "time.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/*Put your SSID & Password*/
const char* ssid = "_Naruto_";  // Enter SSID here
const char* password = "WakeUpNow@5";  //Enter Password here

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;
struct tm timeinfo;
int hours = 0;
int minuites = 0;

struct Task{
  int h;
  int m;
  bool isActive;
  bool isOn;
  int relay;
  char* title;
};
int taskCapacity = 50;
int taskLength = 0;
struct Task taskList[50];

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup(){
  Serial.begin(115200);
  initDisplay();
  initWifi();

  initTime();

  Task newTask = {2,22,true, true, 1, "Something"};
  Task newTask2 = {3,33,true, true, 1, "Something 2"};
  insertTask(newTask);
  insertTask(newTask2);
  listTask();
}

void loop(){
  delay(1000);
  printLocalTime();
}

void initTime(){
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
}

void initDisplay(){
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
}

void initWifi(){
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
}

void printLocalTime(){
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }

  hours = timeinfo.tm_hour;
  minuites = timeinfo.tm_min;
  Serial.println(minuites);
  Serial.println(hours);
  
  Serial.println(&timeinfo, "%d %B %Y");
  Serial.println(&timeinfo, "%H:%M:%S");
  Serial.println(&timeinfo, "%A");
  Serial.println();

  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.println(&timeinfo, "%d %b %H:%M:%S %a");
  display.display();
}

int insertTask(struct Task newTask) 
{ 
    if (taskLength >= taskCapacity) 
    return -1;   
    taskList[taskLength] = newTask; 
    taskLength = taskLength + 1;
    return (taskLength); 
}
int findTask(int hours, int minuites, int relay) 
{ 
    int i; 
    for (i = 0; i < taskLength; i++) 
        if (taskList[i].h == hours && taskList[i].h == minuites && taskList[i].relay == relay) 
            return i; 
  
    return -1; 
}
int deleteElement(int hours, int minuites, int relay) 
{  
    // Find position of element to be deleted  
    int pos = findTask(hours, minuites, relay);
  
    if (pos == - 1)  
    {  
        Serial.println("Element not found");
        return -1;  
    }  
  
  deleteElementAt(pos);
  return taskLength;  
}  

int deleteElementAt(int pos) 
{  
    int i;  
    for (i = pos; i < taskLength - 1; i++)  
        taskList[i] = taskList[i + 1];  
    taskLength = taskLength - 1;
    return taskLength;  
}  

void listTask(){
    int i;  
    for (i = 0; i < taskLength; i++)  
    {
      Serial.print("Title: ");
      Serial.print(taskList[i].title);
      Serial.print(" HH:MM ");
      Serial.print(taskList[i].h);
      Serial.print(":");
      Serial.print(taskList[i].m);
      Serial.println();
    }  
}
