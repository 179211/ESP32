#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DanceBitMap.h>
#include <DoubtBitMap.h>

#define SCREEN_I2C_ADDR 0x3C // or 0x3C
#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
#define OLED_RST_PIN -1      // Reset pin (-1 if not available)
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RST_PIN);


#define FRAME_DELAY (100)
#define FRAME_WIDTH (96)
#define FRAME_HEIGHT (64)

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_I2C_ADDR);
}

int frame = 0;
int i = 0;
void loop() {
  showDance();
  showDoubt();
}

void showDance(){
  for (i = 0; i < 5; i++){
    for (frame = 0; frame < bitmap_dance_allArray_LEN; frame++){
      display.clearDisplay();
      display.drawBitmap(16, 0, bitmap_dance_allArray[frame], FRAME_WIDTH, FRAME_HEIGHT, 1);
      display.display();
      //frame = (frame + 1) % FRAME_COUNT_Dance;
      delay(FRAME_DELAY);
    }
  }
}

void showDoubt(){
  for (i=0; i<5; i++){
      for (frame = 0; frame < doubt_bitmap_allArray_LEN; frame++){
        display.clearDisplay();
        display.drawBitmap(16, 0, doubt_bitmap_allArray[frame], FRAME_WIDTH, FRAME_HEIGHT, 1);
        display.display();
        //frame = (frame + 1) % FRAME_COUNT_Doubt;
        delay(FRAME_DELAY + 50);
      }
  }
}