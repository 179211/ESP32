#include <SPI.h>
#include <nRF24L01.h>
#include <Wire.h>

#define PIN_SW1 4
#define PIN_SW2 16
#define PIN_SW3 27
#define PIN_SW4 26
#define PIN_SW7 2   //t

#define PIN_V_CE 17
#define PIN_V_CSN 8
#define PIN_H_CE 25
#define PIN_H_CSN 8

#define GPIO_13 13  // MOSI
#define GPIO_14 14  // CLK
#define GPIO_15 15  // CSO

#define PIN_POTLX 36
#define PIN_POTLY 39
#define PIN_POTRX 34
#define PIN_POTRY 35
#define PIN_POTL 32
#define PIN_POTR 33

struct Data_Package
{
    byte lxVal;
    byte lyVal;
    byte rxVal;
    byte ryVal;
    byte potLVal;
    byte potRVal;
    byte SW1;
    byte SW2;
    byte SW3;
    byte SW4;
    byte SW5;
    byte SW6;
    byte SW7;
};
Data_Package data;

void initData()
{
    data.lxVal = 127;
    data.lyVal = 127;
    data.rxVal = 127;
    data.ryVal = 127;
    data.potLVal = 127;
    data.potRVal = 127;
    data.SW1 = 1;
    data.SW2 = 1;
    data.SW3 = 1;
    data.SW4 = 1;
    data.SW5 = 1;
    data.SW6 = 1;
    data.SW7 = 1;
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
    readPots();
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
    //   data.SW5 = digitalRead(PIN_SW5);
    //   data.SW6 = digitalRead(PIN_SW6);
    data.SW7 = digitalRead(PIN_SW7);

    Serial.println("BTN:");
    Serial.println(data.SW1);
    Serial.println(data.SW2);
    Serial.println(data.SW3);
    Serial.println(data.SW4);
    Serial.println(data.SW5);
    Serial.println(data.SW6);
    Serial.println(data.SW7);
}

void readPots(){
  data.lxVal = map(analogRead(PIN_POTLX), 0, 4095, 0, 255);
  data.lyVal = map(analogRead(PIN_POTLY), 0, 4095, 0, 255);
  data.rxVal = map(analogRead(PIN_POTRX), 0, 4095, 0, 255);
  data.ryVal = map(analogRead(PIN_POTRY), 0, 4095, 0, 255);
  data.potLVal = map(analogRead(PIN_POTL), 0, 4095, 0, 255);
  data.potRVal = map(analogRead(PIN_POTR), 0, 4095, 0, 255);

  Serial.println("POT:");
  Serial.println(data.lxVal);
  Serial.println(data.lyVal);
  Serial.println(data.rxVal);
  Serial.println(data.ryVal);
  Serial.println(data.potLVal);
  Serial.println(data.potRVal);

  //Max 4095
  //1935-120
  //int i = map(analogRead(sensorPin), 0, 1023, 0, 255);
  //int sensorValue2 = map(analogRead(sensorPin), 0, 4095, 0, 255);
}

void setUpPins(){
  pinMode(PIN_SW1, INPUT_PULLUP);
  pinMode(PIN_SW2, INPUT_PULLUP);
  pinMode(PIN_SW3, INPUT_PULLUP);
  pinMode(PIN_SW4, INPUT_PULLUP);
//   pinMode(PIN_SW5, INPUT_PULLUP);
//   pinMode(PIN_SW6, INPUT_PULLUP);
  pinMode(PIN_SW7, INPUT_PULLUP);
}

void initDisplay(){
  Serial.begin(115200);
  delay(2000);
  Serial.println ("Started");
  Serial.println ("*************************************");
}
