/*
  Simple example for receiving

  https://github.com/sui77/rc-switch/
*/

#include <RCSwitch.h>

#define RF_PIN 16
#define Led_PIN 2

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(115200);
  delay(100);

  mySwitch.enableReceive(digitalPinToInterrupt(RF_PIN));
  //mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2

  pinMode(Led_PIN, OUTPUT);
  digitalWrite(Led_PIN, HIGH);
  delay(1000);
  digitalWrite(Led_PIN, LOW);
  delay(1000);
  digitalWrite(Led_PIN, HIGH);
  delay(1000);
  digitalWrite(Led_PIN, LOW);

  Serial.print("Started");
  Serial.println ("*************************************");
}

void loop() {
  digitalWrite(Led_PIN, LOW);
  if (mySwitch.available()) {
    digitalWrite(Led_PIN, HIGH);
    delay(500);
    Serial.print("Value: ");
    Serial.print( mySwitch.getReceivedValue() );
    Serial.print(" / ");
    Serial.print("Bitlength: ");
    Serial.print( mySwitch.getReceivedBitlength() );
    Serial.print("bit ");
    Serial.print(" / ");
    Serial.print("Protocol: ");
    Serial.println( mySwitch.getReceivedProtocol() );
    
    mySwitch.resetAvailable();
  }
}
