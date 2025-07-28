#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeSansBoldOblique12pt7b.h>
#include <Fonts/FreeMono18pt7b.h>    
#include <Fonts/FreeSansBoldOblique18pt7b.h>
#include <Fonts/FreeMono24pt7b.h>   
#include <Fonts/FreeSansBoldOblique24pt7b.h>
#include <Fonts/FreeMono9pt7b.h>      
#include <Fonts/FreeSansBoldOblique9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>   
#include <Fonts/FreeSansOblique12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>   
#include <Fonts/FreeSansOblique18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>   
#include <Fonts/FreeSansOblique24pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>    
#include <Fonts/FreeSansOblique9pt7b.h>
#include <Fonts/FreeMonoBoldOblique12pt7b.h>  
#include <Fonts/FreeSerif12pt7b.h>
#include <Fonts/FreeMonoBoldOblique18pt7b.h>  
#include <Fonts/FreeSerif18pt7b.h>
#include <Fonts/FreeMonoBoldOblique24pt7b.h>  
#include <Fonts/FreeSerif24pt7b.h>
#include <Fonts/FreeMonoBoldOblique9pt7b.h> 
#include <Fonts/FreeSerif9pt7b.h>
#include <Fonts/FreeMonoOblique12pt7b.h>    
#include <Fonts/FreeSerifBold12pt7b.h>
#include <Fonts/FreeMonoOblique18pt7b.h>    
#include <Fonts/FreeSerifBold18pt7b.h>
#include <Fonts/FreeMonoOblique24pt7b.h>    
#include <Fonts/FreeSerifBold24pt7b.h>
#include <Fonts/FreeMonoOblique9pt7b.h>   
#include <Fonts/FreeSerifBold9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>   
#include <Fonts/FreeSerifBoldItalic12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>   
#include <Fonts/FreeSerifBoldItalic18pt7b.h>
#include <Fonts/FreeSans24pt7b.h>   
#include <Fonts/FreeSerifBoldItalic24pt7b.h>
#include <Fonts/FreeSans9pt7b.h>      
#include <Fonts/FreeSerifBoldItalic9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>   
#include <Fonts/FreeSerifItalic12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>   
#include <Fonts/FreeSerifItalic18pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>   
#include <Fonts/FreeSerifItalic24pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>    
#include <Fonts/FreeSerifItalic9pt7b.h>

void setup(){
  Serial.begin(115200);
  initDisplay();
}

void loop(){
  delay(1000);
  printMSG();
}

void initDisplay(){
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(1000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
}

void printMSG(){

display.setFont(&FreeMono12pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);    
display.setFont(&FreeSansBoldOblique12pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);
display.setFont(&FreeMono18pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);   
display.setFont(&FreeSansBoldOblique18pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);
display.setFont(&FreeMono24pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);   
display.setFont(&FreeSansBoldOblique24pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);
display.setFont(&FreeMono9pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);      
display.setFont(&FreeSansBoldOblique9pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);
display.setFont(&FreeMonoBold12pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);   
display.setFont(&FreeSansOblique12pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);
display.setFont(&FreeMonoBold18pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);   
display.setFont(&FreeSansOblique18pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);
display.setFont(&FreeMonoBold24pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);   
display.setFont(&FreeSansOblique24pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);
display.setFont(&FreeMonoBold9pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);    
display.setFont(&FreeSansOblique9pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);
display.setFont(&FreeMonoBoldOblique12pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);  
display.setFont(&FreeSerif12pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);
display.setFont(&FreeMonoBoldOblique18pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);  
display.setFont(&FreeSerif18pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);
display.setFont(&FreeMonoBoldOblique24pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);  
display.setFont(&FreeSerif24pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);
display.setFont(&FreeMonoBoldOblique9pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000); 
display.setFont(&FreeSerif9pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);
display.setFont(&FreeMonoOblique12pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);    
display.setFont(&FreeSerifBold12pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);
display.setFont(&FreeMonoOblique18pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);    
display.setFont(&FreeSerifBold18pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);
display.setFont(&FreeMonoOblique24pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);    
display.setFont(&FreeSerifBold24pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);
display.setFont(&FreeMonoOblique9pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);   
display.setFont(&FreeSerifBold9pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);
display.setFont(&FreeSans12pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);   
display.setFont(&FreeSerifBoldItalic12pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);
display.setFont(&FreeSans18pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);   
display.setFont(&FreeSerifBoldItalic18pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);
display.setFont(&FreeSans24pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);   
display.setFont(&FreeSerifBoldItalic24pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);
display.setFont(&FreeSans9pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);      
display.setFont(&FreeSerifBoldItalic9pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);
display.setFont(&FreeSansBold12pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);   
display.setFont(&FreeSerifItalic12pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);
display.setFont(&FreeSansBold18pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);   
display.setFont(&FreeSerifItalic18pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);
display.setFont(&FreeSansBold24pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);   
display.setFont(&FreeSerifItalic24pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);
display.setFont(&FreeSansBold9pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);    
display.setFont(&FreeSerifItalic9pt7b);display.clearDisplay();display.setCursor(0,0);display.setTextSize(1);display.println("Mani");display.display();delay(2000);
}
