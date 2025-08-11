#include <MyDisplay.h>

/// Display configuration
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
extern Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
extern MyDisplay myDisplay;

void setup()
{
}

void loop()
{
  myDisplay.showMsg("Hello World");
  myDisplay.showMsg("Hello World", 3);
}