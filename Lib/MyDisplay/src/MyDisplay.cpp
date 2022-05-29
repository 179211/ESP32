#include "MyDisplay.h"

MyDisplay::MyDisplay(bool isDisplayAvailable)
{
  Serial.begin(115200); delay(1000); //Take some time to open up the Serial Monitor

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    _isDisplayAvailable = false;
    //for(;;); // Don't proceed, loop forever
  }
  _isDisplayAvailable = true;

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

void MyDisplay::showMsg(String msg){
  if (_isDisplayAvailable){
      display.clearDisplay();
      display.setCursor(0,0);
      display.println(msg);
      display.display();
      delay(2000);    
  }
  else{    
    Serial.println(msg);
  }
}
