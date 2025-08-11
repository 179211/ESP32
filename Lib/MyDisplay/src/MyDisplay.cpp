#include "MyDisplay.h"

MyDisplay::MyDisplay(uint8_t addr)
{
  Serial.begin(115200);
  while (!Serial) {}  // some boards need to wait to ensure access to serial over USB

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, addr)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    _isDisplayAvailable = false;
    //for(;;); // Don't proceed, loop forever
  }

  if(_isDisplayAvailable){
    display.display();
    delay(2000);

    // Clear the buffer
    display.clearDisplay();
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 0);     // Start at top-left corner
    showMsg("Display Started");
  }
}

void MyDisplay::showMsg(String msg, int delaySec){
  if (_isDisplayAvailable){
      display.clearDisplay();
      display.setCursor(0,0);
      display.println(msg);
      display.display();
  }
  Serial.println(msg);
  delay(delaySec * 1000);    
}
