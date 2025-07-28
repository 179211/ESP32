/// NRF24Transmitter 

//Display
//GPIO 22 (SCL) and GPIO 21 (SDA)
//RF
//NRF24L01 VCC ………………………………………… 3.3V of ESP32
//NRF24L01 CSN ………………………………………… D5 of ESP32
//NRF24L01 MOSI ………………………………………… D23 of ESP32
//NRF24L01 GND ………………………………………… GND of ESP32
//NRF24L01 CE ………………………………………… D4 of ESP32
//NRF24L01 SCK ………………………………………… D18 of ESP32
//NRF24L01 MISO ………………………………………… D19 of ESP32

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <nRF24L01.h>
#include <RF24.h>

#define PIN_POTLX 36
#define PIN_POTLY 39
#define PIN_POTRX 34
#define PIN_POTRY 35
#define PIN_POTL 27
#define PIN_POTR 13

#define PIN_SW1 25
#define PIN_SW2 33
#define PIN_SW3 32
#define PIN_SW4 14
#define PIN_SW5 15
#define PIN_SW6 3
#define PIN_SW7 26

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

RF24 radio(4, 5); // ESP32
//RF24 radio(7, 8); // Arduino CE, CSN
const byte address[6] = "00001";

int curVal = 1;
String serialVal = "";
String radioVal = "";
boolean isRadioSender = 1;

int i=0;
bool swList[8] = {1, 1, 1, 1, 1, 1, 1, 1};
byte swNo=0;

byte dlxVal = 0;
byte dlyVal = 0;
byte drxVal = 0;
byte dryVal = 0;

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte lxVal;
  byte lyVal;
  byte rxVal;
  byte ryVal;
  byte potLVal;
  byte potRVal;
  byte _SW;
};
Data_Package data; //Create a variable with the above structure
void initData(){
  data.lxVal = 127;
  data.lyVal = 127;
  data.rxVal = 127;
  data.ryVal = 127;
  data.potLVal = 127;
  data.potRVal = 127;
  data._SW = 255;

  delay(1000);
  readPots();
  dlxVal = 127 - data.lxVal;
  dlyVal = 127 - data.lyVal;
  drxVal = 127 - data.rxVal;
  dryVal = 127 - data.ryVal;

  Serial.println("Delta");
  Serial.println(dlxVal);
  Serial.println(dlyVal);
  Serial.println(drxVal);
  Serial.println(dryVal);
  delay(1000);

  readPots();
}

void setup() {
  initDisplay();
  setUpPins();
  initData();
  initRadio();
  if (isRadioSender == HIGH) {
    startRadioSender();
  }
  else {
    startRadioReceiver();
  }  
}

void setUpPins(){
  pinMode(PIN_SW1, INPUT_PULLUP);
  pinMode(PIN_SW2, INPUT_PULLUP);
  pinMode(PIN_SW3, INPUT_PULLUP);
  pinMode(PIN_SW4, INPUT_PULLUP);
  pinMode(PIN_SW5, INPUT_PULLUP);
  pinMode(PIN_SW6, INPUT_PULLUP);
  pinMode(PIN_SW7, INPUT_PULLUP);
  //pinMode(PIN_SW8, INPUT_PULLUP);
}

void readPins(){
  swList[0] = digitalRead(PIN_SW1);
  swList[1] = digitalRead(PIN_SW2);
  swList[2] = digitalRead(PIN_SW3);
  swList[3] = digitalRead(PIN_SW4);
  swList[4] = digitalRead(PIN_SW5);
  swList[5] = digitalRead(PIN_SW6);
  swList[6] = digitalRead(PIN_SW7);
  swList[7] = 0;
  encodeBtn();
  data._SW = swNo;
  Serial.println("BTN:");
  Serial.println(swNo);
}

void encodeBtn(){
  swNo = 0;
  swNo += swList[0] << 7;
  swNo += swList[1] << 6;
  swNo += swList[2] << 5;
  swNo += swList[3] << 4;
  swNo += swList[4] << 3;
  swNo += swList[5] << 2;
  swNo += swList[6] << 1;
  swNo += swList[7] << 0;
  Serial.println("swNo ");
  Serial.println(swNo);
}

void decodeBtn(){
  byte recByte = swNo;  
  swNo = 0;
  swList[0] = recByte >> 7;
  swNo += swList[0]*128;
  swList[1] = (recByte-swNo) >> 6;
  swNo += swList[1]*64;
  swList[2] = (recByte-swNo) >> 5;
  swNo += swList[2]*32;
  swList[3] = (recByte-swNo) >> 4;
  swNo += swList[3]*16;
  swList[4] = (recByte-swNo) >> 3;
  swNo += swList[4]*8;
  swList[5] = (recByte-swNo) >> 2;
  swNo += swList[5]*4;
  swList[6] = (recByte-swNo) >> 1;
  swNo += swList[6]*2;
  swList[7] = (recByte-swNo) >> 0;
  swNo += swList[7]*0;

  Serial.println("swList ");
  for(i=0; i<8; i++){
    Serial.print(swList[i]);
  }
  Serial.println("");
}

void readPots(){
  i = map(analogRead(PIN_POTLX), 0, 4095, 0, 255)+dlxVal; if (i > 255){ i = 255; } else if (i < 0){ i = 0; } data.lxVal = i;
  i = map(analogRead(PIN_POTLY), 0, 4095, 0, 255)+dlyVal; if (i > 255){ i = 255; } else if (i < 0){ i = 0; } data.lyVal = i;
  i = map(analogRead(PIN_POTRX), 0, 4095, 0, 255)+drxVal; if (i > 255){ i = 255; } else if (i < 0){ i = 0; } data.rxVal = i;
  i = map(analogRead(PIN_POTRY), 0, 4095, 0, 255)+dryVal; if (i > 255){ i = 255; } else if (i < 0){ i = 0; } data.ryVal = i;

  // data.lxVal = map(analogRead(PIN_POTLX), 0, 4095, 0, 255);
  // data.lyVal = map(analogRead(PIN_POTLY), 0, 4095, 0, 255);
  // data.rxVal = map(analogRead(PIN_POTRX), 0, 4095, 0, 255);
  // data.ryVal = map(analogRead(PIN_POTRY), 0, 4095, 0, 255);
  data.potLVal = map(analogRead(PIN_POTL), 0, 4095, 0, 255);
  data.potRVal = map(analogRead(PIN_POTR), 0, 4095, 0, 255);
  
  Serial.println("POT: ");
  Serial.println(data.lxVal);
  Serial.println(data.lyVal);
  Serial.println(data.rxVal);
  Serial.println(data.ryVal);
  Serial.println(data.potLVal);
  Serial.println(data.potRVal);

  //int i = map(analogRead(sensorPin), 0, 1023, 0, 255);
  //int i = map(analogRead(sensorPin), 0, 4095, 0, 255);
}

void showData(){
  Serial.println("POT: ");
  Serial.println(data.lxVal);
  Serial.println(data.lyVal);
  Serial.println(data.rxVal);
  Serial.println(data.ryVal);
  Serial.println(data.potLVal);
  Serial.println(data.potRVal);
  swNo = data._SW;
  decodeBtn();
  showDataScr();
}

void showDataScr(){
  display.clearDisplay();
  display.setCursor(0,0);
  display.print(F("X"));
  display.print(data.lxVal);
  display.setCursor(40,0);
  display.print(F("PL "));
  display.print(data.potLVal);
  display.setCursor(90,0);
  display.print(F("X"));
  display.print(data.rxVal);
  display.setCursor(0,10);
  display.print(F("Y"));
  display.print(data.lyVal);
  display.setCursor(40, 10);
  display.print(F("PR "));
  display.print(data.potRVal);
  display.setCursor(90, 10);
  display.print(F("Y"));
  display.print(data.ryVal);
  
  display.setCursor(0, 25);
  for(i=0; i<8; i++){
    display.print(swList[i]);
    display.print(F("  "));
  }
  display.display();
  //delay(1000);
}

void startRadioReceiver(){
  radio.openReadingPipe(0, address);
  radio.startListening();
}

void startRadioSender(){
  radio.openWritingPipe(address);
  radio.stopListening();
  const char text[] = "Hello Tushar";
  radio.write(&text, sizeof(text));
  delay(1000);
}

void initRadio(){
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);
}

void initDisplay(){
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.setRotation(2);
  display.display();
  delay(2000); // Pause for 2 seconds
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.clearDisplay();
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Hello, world!"));
  display.display();
  
  delay(2000);
  Serial.println ("Started");
  Serial.println ("*************************************");
}

void loop() {
  showData();
  radioVal="";
  readPots();
  readPins();
  
  if (isRadioSender == HIGH) {
    curVal++;
    
    //const char text[] = "Hello World";
    //radio.write(&text, sizeof(text));
    //Serial.println(text);
  
    serialVal = "Tushar " + String(curVal);
    Serial.println("S: "+serialVal);
    radio.write(&serialVal, sizeof(serialVal));
    
    // Send the whole data from the structure to the receiver
    //radio.write(&data, sizeof(Data_Package));

    while (Serial.available() > 0)
    {
      serialVal = Serial.readString();
      serialVal.trim();
      Serial.println(serialVal);
      radio.write(&serialVal, sizeof(serialVal));
    }

    display.clearDisplay();
    display.setCursor(0,0);
    display.println(F(&serialVal));
    display.display();
    //delay(1000);
  }
  else {
    if (radio.available()) {
      //char text[32] = "";
      //radio.read(&text, sizeof(text));
      //Serial.println(text);

      // Send the whole data from the structure to the receiver
      //radio.read(&data, sizeof(Data_Package));
      //showData();

      radio.read(&radioVal, sizeof(radioVal));
      Serial.println("R: "+radioVal);
      display.clearDisplay();
      display.setCursor(0,0);
      display.println(F(&radioVal));
      display.display();    
    }
    else{
      radioVal = "";
    }
  }
}
