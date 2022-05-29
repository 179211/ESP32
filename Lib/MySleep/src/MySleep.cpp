#include "MySleep.h"

MySleep::MySleep(int dummy)
{
  initSleep();
}

void MySleep::initSleep(){  
    ++bootCount;
    Serial.println("Boot number: " + String(bootCount));
    print_wakeup_reason();
    esp_sleep_enable_touchpad_wakeup();
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");
}

void MySleep::goToSleep(int timeToSleepInSec){
  Serial.println("Going to sleep now for " + String(timeToSleepInSec) + " Seconds");
  String msg = "Going to sleep now for " + String(timeToSleepInSec) + " Seconds"; 
  myDisplay.showMsg(msg);
  Serial.flush(); 
  esp_sleep_enable_timer_wakeup(timeToSleepInSec * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

void TouchCallback()
{
}

void MySleep::enableTouch(uint8_t pinTouch, bool b)
{
    if(b){
        ptr = &TouchCallback;
        touchAttachInterrupt(pinTouch, ptr, Threshold);
    }
}

void MySleep::print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : 
      Serial.println("Wakeup caused by touchpad"); 
      print_wakeup_touchpad();
      break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

void MySleep::print_wakeup_touchpad(){
  touch_pad_t touchPin = esp_sleep_get_touchpad_wakeup_status();
  switch(touchPin)
  {
    case 0  : Serial.println("Touch 0 detected on GPIO 4"); break;
    case 1  : Serial.println("Touch 1 detected on GPIO 0"); break;
    case 2  : Serial.println("Touch 2 detected on GPIO 2"); break; //connected to internal led don't use
    case 3  : Serial.println("Touch 3 detected on GPIO 15"); break;
    case 4  : Serial.println("Touch 4 detected on GPIO 13"); break;
    case 5  : Serial.println("Touch 5 detected on GPIO 12"); break;
    case 6  : Serial.println("Touch 6 detected on GPIO 14"); break;
    case 7  : Serial.println("Touch 7 detected on GPIO 27"); break;
    case 8  : Serial.println("Touch 8 detected on GPIO 33"); break;
    case 9  : Serial.println("Touch 9 detected on GPIO 32"); break;
    default : Serial.println("Wakeup not by touchpad"); break;
  }
}
