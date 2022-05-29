#include <MyWifi.h>
#define OnBoardLed 2

/// Display configuration
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
extern Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
extern MyDisplay myDisplay(true);

MyWifi myWifi(OnBoardLed); // On board led
struct tm timeinfo;

void setup()
{
  myDisplay.showMsg("Hello World");
  char *body = "<b>Hello World!</b><br><p>This is an <i>HTML email</i> body";
  String result = myWifi.sendEmail("Test", body, true);
  if (result == ""){
    myDisplay.showMsg("Email Sent");
  }
  else{
    myDisplay.showMsg(result);
  }

}

void loop()
{
  checkWifi();
}

void checkWifi(){
  if (WiFi.status() != WL_CONNECTED){
    if (OnBoardLed > 0){
      digitalWrite(OnBoardLed, LOW);
    }
    if (timeinfo.tm_hour != myWifi.wifiLastRetry){
      myWifi.wifiLastRetry = timeinfo.tm_hour;
      myWifi.initWifi();
    }
  }
}
