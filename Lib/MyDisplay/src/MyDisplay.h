#ifndef MyDisplay_H
#define MyDisplay_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;

class MyDisplay
{
  private:
    bool _isDisplayAvailable = false;

  public:
    MyDisplay(bool isDisplayAvailable);
    void showMsg(String msg);
};

#endif
