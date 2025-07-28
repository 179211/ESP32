//https://github.com/colonelwatch/ESP32-oled-spectrum/tree/master
//Arduino Spectrum Analyzer
//=========================
  #include <FreqCount.h>
  #include "fix_fft.h"
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>

  #define SAMPLES 128
  #define AUDIOPIN A0
  #define FRQPIN D5

  Adafruit_SSD1306 display(128, 64, &Wire, 4);
  char im[SAMPLES], data[SAMPLES]; char x = 0, ylim = 60; int i = 0, val;

  void setup()
  {
    Serial.begin(9600);
    display.begin(SSD1306_SWITCHCAPVCC,0x3C);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.clearDisplay();
    analogReference(DEFAULT);
    FreqCount.begin(1000);
}
void loop()
{
  if (FreqCount.available()) {
    unsigned long count = FreqCount.read();
    Serial.println(count);
  }

  for (i=0; i < SAMPLES; i++)     //take 128 samples
  {
    val = analogRead(AUDIOPIN);
    data[i] = val/4 - 128;
    im[i] = 0;
  }

  fix_fft(data, im, 7, 0);    //FFT on data

  display.clearDisplay();
  int dataMax = 0;
  int x0 = 0;
  int y0 = 0;
  int xMax = 0;
  int dat = 0; 
  for (i=1; i<64; i++) //bar graph sketch on OLED display
  {
    Serial.print("Dat ");
    dat = sqrt(data[i] * data[i] + im[i] * im[i]);
    Serial.print(dat);

    Serial.print(" x0  ");
    x0 = i*2 + x;
    Serial.println(x0);

    y0 = ylim - dat;
    if(dat > dataMax) {
      dataMax = dat;
      xMax = x0;
    }
    display.drawLine(x0, ylim, x0, y0, WHITE);
  }

  display.setCursor(0,0); 
  display.print(xMax * 4);

  Serial.print("-------------- dataMax ");
  Serial.print(dataMax);
  Serial.print("  xMax  ");
  Serial.println(xMax);
  display.display();
}
