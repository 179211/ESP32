#include <SPI.h>
#include <Wire.h>

#define PIN_SW1 25
#define PIN_SW2 33
#define PIN_SW3 32
#define PIN_SW4 14
#define PIN_SW5 15
#define PIN_SW6 3
#define PIN_SW7 26

const int PIN_POTLX = 36;
const int PIN_POTLY = 39;
const int PIN_POTRX = 34;
const int PIN_POTRY = 35;
const int PIN_POTR = 13;
const int PIN_POTL = 27;

int lxVal = 0;
int lyVal = 0;
int rxVal = 0;
int ryVal = 0;
int potLVal = 0;
int potRVal = 0;

int _SW1 = 1;
int _SW2 = 1;
int _SW3 = 1;
int _SW4 = 1;
int _SW5 = 1;
int _SW6 = 1;
int _SW7 = 1;

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte lxVal;
  byte lyVal;
  byte rxVal;
  byte ryVal;
  byte potLVal;
  byte potRVal;
  byte _SW1;
  byte _SW2;
  byte _SW3;
  byte _SW4;
  byte _SW5;
  byte _SW6;
  byte _SW7;
};
Data_Package data; //Create a variable with the above structure

void initData(){
  data.lxVal = 127;
  data.lyVal = 127;
  data.rxVal = 127;
  data.ryVal = 127;
  data.potLVal = 127;
  data.potRVal = 127;
  data._SW1 = 1;
  data._SW2 = 1;
  data._SW3 = 1;
  data._SW4 = 1;
  data._SW5 = 1;
  data._SW6 = 1;
  data._SW7 = 1;
}

void setup() {
  initDisplay();
  setUpPins();
  initData();
}

void loop() {
  readPots();
  //readPins();
  Serial.println("SIZE: ");  
  Serial.println(sizeof(Data_Package));  
}

void readPins(){
  _SW1 = digitalRead(PIN_SW1);
  _SW2 = digitalRead(PIN_SW2);
  _SW3 = digitalRead(PIN_SW3);
  _SW4 = digitalRead(PIN_SW4);
  _SW5 = digitalRead(PIN_SW5);
  _SW6 = digitalRead(PIN_SW6);
  _SW7 = digitalRead(PIN_SW7);
  
  Serial.println("BTN:");
  Serial.println(_SW1);
  Serial.println(_SW2);
  Serial.println(_SW3);
  Serial.println(_SW4);
  Serial.println(_SW5);
  Serial.println(_SW6);
  Serial.println(_SW7);

  data._SW1 = _SW1;
  data._SW2 = _SW2;
  data._SW3 = _SW3;
  data._SW4 = _SW4;
  data._SW5 = _SW5;
  data._SW6 = _SW6;
  data._SW7 = _SW7;

  delay(1000);
}

void setUpPins(){
  pinMode(PIN_SW1, INPUT_PULLUP);
  pinMode(PIN_SW2, INPUT_PULLUP);
  pinMode(PIN_SW3, INPUT_PULLUP);
  pinMode(PIN_SW4, INPUT_PULLUP);
  pinMode(PIN_SW5, INPUT_PULLUP);
  pinMode(PIN_SW6, INPUT_PULLUP);
  pinMode(PIN_SW7, INPUT_PULLUP);
}

void readPots(){
  lxVal = analogRead(PIN_POTLX);
  lyVal = analogRead(PIN_POTLY);
  rxVal = analogRead(PIN_POTRX);
  ryVal = analogRead(PIN_POTRY);
  potLVal = analogRead(PIN_POTL);
  potRVal = analogRead(PIN_POTR);

  data.lxVal = map(lxVal, 0, 4095, 0, 255);
  data.lyVal = map(lyVal, 0, 4095, 0, 255);
  data.rxVal = map(rxVal, 0, 4095, 0, 255);
  data.ryVal = map(ryVal, 0, 4095, 0, 255);
  data.potLVal = map(potLVal, 0, 4095, 0, 255);
  data.potRVal = map(potRVal, 0, 4095, 0, 255);

  Serial.println("POT:");
  Serial.println(data.lxVal);
  Serial.println(data.lyVal);
  Serial.println(data.rxVal);
  Serial.println(data.ryVal);
  Serial.println(data.potLVal);
  Serial.println(data.potRVal);
  delay(1000);

  //Max 4095
  //1935-120
  //int i = map(analogRead(sensorPin), 0, 1023, 0, 255);
  //int sensorValue2 = map(analogRead(sensorPin), 0, 4095, 0, 255);
}

void initDisplay(){
  Serial.begin(115200);
  delay(2000);
  Serial.println ("Started");
  Serial.println ("*************************************");
}
