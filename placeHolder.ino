#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include "time.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP_Mail_Client.h>

int OnBoardLed = 2;

#define NUM_RELAYS 8
int relayGPIOs[NUM_RELAYS] = {4, 16, 17, 18, 19, 23, 27, 13};
// I/P = 36, 39, 34, 35, 32, 33, 25, 26

String msg = "";

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
  pinMode(OnBoardLed, OUTPUT);

  Serial.begin(115200);
  delay(1000); // Take some time to open up the Serial Monitor

  Serial.println("\n_initDisplay");
  initDisplay();

}

void loop()
{
    
}

void initDisplay()
{
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(2);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);             // Start at top-left corner
}

void showMsg(String msg)
{
  Serial.println(msg);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(msg);
  display.display();
  delay(2000);
}
