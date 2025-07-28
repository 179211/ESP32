#include <SPI.h>
#include <nRF24L01.h>
#include <Wire.h>

#define PIN_SW1 4
#define PIN_SW2 16
#define PIN_SW3 27
#define PIN_SW4 26

struct Data_Package
{
    byte SW1;
    byte SW2;
    byte SW3;
    byte SW4;
};
Data_Package data;

void initData()
{
    data.SW1 = 1;
    data.SW2 = 1;
    data.SW3 = 1;
    data.SW4 = 1;
}

void setup()
{
    initDisplay();
    setUpPins();
    initData();
}

void loop()
{
    readPins();
    Serial.println("SIZE: ");
    Serial.println(sizeof(Data_Package));
  delay(1000);
}

void readPins()
{
    data.SW1 = digitalRead(PIN_SW1);
    data.SW2 = digitalRead(PIN_SW2);
    data.SW3 = digitalRead(PIN_SW3);
    data.SW4 = digitalRead(PIN_SW4);

    Serial.println("BTN:");
    Serial.println(data.SW1);
    Serial.println(data.SW2);
    Serial.println(data.SW3);
    Serial.println(data.SW4);
}

void setUpPins(){
  pinMode(PIN_SW1, INPUT_PULLUP);
  pinMode(PIN_SW2, INPUT_PULLUP);
  pinMode(PIN_SW3, INPUT_PULLUP);
  pinMode(PIN_SW4, INPUT_PULLUP);
}

void initDisplay(){
  Serial.begin(115200);
  delay(2000);
  Serial.println ("Started");
  Serial.println ("*************************************");
}
