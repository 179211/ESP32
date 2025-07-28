// Esp 32

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(17, 5); // CE, CSN

const byte address[6] = "00001";
float payload = 0.0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    // some boards need to wait to ensure access to serial over USB
  }
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  Serial.println(radio.getChannel());
  bool report = radio.write(&payload, sizeof(float));  // transmit & save the report
  if (report) {
    Serial.print(F("Transmission successful! "));  // payload was delivered
    Serial.print(payload);  // print payload sent
    Serial.print("  ");
    Serial.println(sizeof(payload));

    payload += 0.01;          // increment float payload

  } else {
    Serial.println(F("Transmission failed or timed out"));  // payload was not delivered
  }
  delay(1000);
}