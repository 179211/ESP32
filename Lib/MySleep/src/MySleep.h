#ifndef MySleep_H
#define MySleep_H

  #include <Arduino.h>
  #include <MyDisplay.h>
  extern MyDisplay myDisplay;

  #define TIME_TO_SLEEP  15        /* Time ESP32 will go to sleep (in seconds) */
  #define Threshold 40 /* Greater the value, more the sensitivity */
  #define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
  extern RTC_DATA_ATTR int bootCount;

class MySleep
{
  private:
    void (*ptr)();
    void print_wakeup_touchpad();
    void print_wakeup_reason();

  public:
    MySleep(int dummy);
    void initSleep();
    void goToSleep(int timeToSleepInSec);
    void enableTouch(uint8_t pin, bool b);
};

#endif
