#ifndef MyDisplay_H
#define MyDisplay_H

#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;

class MyDisplay
{
  private:
    bool _isDisplayAvailable = true;

  public:
    MyDisplay(uint8_t addr = 0x3C);
    void showMsg(String msg, int delaySec = 2);
};

#endif
