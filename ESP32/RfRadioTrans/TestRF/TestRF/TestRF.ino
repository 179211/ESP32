#include <SPI.h>
#include <RF24.h>
#include <Wire.h>

#define PIN_V_MO 23
#define PIN_V_MI 19
#define PIN_V_CLK 18
#define PIN_V_CSN 5
#define PIN_V_CE 17

#define PIN_H_MO 13
#define PIN_H_MI 12
#define PIN_H_CLK 14
#define PIN_H_CSN 15
#define PIN_H_CE 25

#define GPIO_13 13  // MOSI
#define GPIO_14 14  // CLK
#define GPIO_15 15  // CSO

RF24 radio(PIN_V_CE, PIN_V_CSN);
const byte address[6] = "00001"; // Address

float payload = 0.0;
bool role = false;  // true = TX role, false = RX role

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    // some boards need to wait to ensure access to serial over USB
  }

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
      Serial.println(payload);  // print the payload's value
    }

  }

}

