#include <SPI.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <nRF24L01.h>

#define ChipEnable 2

#define PIN_V_MO 23
#define PIN_V_MI 19
#define PIN_V_CLK 18
#define PIN_V_CSN 5
#define PIN_V_CE 2

#define PIN_H_MO 13
#define PIN_H_MI 12
#define PIN_H_CLK 14
#define PIN_H_CSN 15
#define PIN_H_CE 4

#define GPIO_13 13  // MOSI
#define GPIO_14 14  // CLK
#define GPIO_15 15  // CSO


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// true = TX role, false = RX role
bool role = true;

RF24 radio(PIN_V_CE, PIN_V_CSN);
const byte address[6] = "00001"; // Address

float payload = 0.0;

void setup() {
  Serial.begin(115200);
  initDisplay();

  pinMode(ChipEnable, OUTPUT);

  // initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}  // hold in infinite loop
  }

  if (role) {
    // put radio in TX mode
    radio.openWritingPipe(address);
    radio.stopListening();  
  } else {
    // put radio in RX mode
    radio.openReadingPipe(0, address);
    radio.startListening();  
  }
  radio.setAutoAck(false);
//  radio.setPALevel(RF24_PA_LOW);

  uint8_t dataRate = 51;
  if (dataRate == 50) {
    Serial.println(F("Using 2 Mbps."));
    radio.setDataRate(RF24_2MBPS);
  } else if (dataRate == 51) {
    Serial.println(F("Using 250 kbps."));
    radio.setDataRate(RF24_250KBPS);
  } else {
    Serial.println(F("Using 1 Mbps."));
    radio.setDataRate(RF24_1MBPS);
  }

}

void loop() {
  if (role) {
    // This device is a TX node
    unsigned long start_timer = micros();                // start the timer
    bool report = radio.write(&payload, sizeof(float));  // transmit & save the report
    unsigned long end_timer = micros();                  // end the timer

    if (report) {
      Serial.print(F("Transmission successful! "));  // payload was delivered
      Serial.print(F("Time to transmit = "));
      Serial.print(end_timer - start_timer);  // print the timer result
      Serial.print(F(" us. Sent: "));
      Serial.println(payload);  // print payload sent
      payload += 0.01;          // increment float payload
    } else {
      Serial.println(F("Transmission failed or timed out"));  // payload was not delivered
    }

  }
  else{
    // This device is a RX node
    if (radio.available()) {
      radio.read(&payload, sizeof(payload)); // Read the whole data and store it into the 'data' structure
      Serial.println(payload);
      display.clearDisplay();
      display.setCursor(0,0);
      display.println(payload);
      display.display();
    }

  }

  delay(1000);

}

void initDisplay(){
  Serial.begin(115200); while (!Serial) {} 
  
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
