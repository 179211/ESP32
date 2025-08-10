/*
  ESC_Stop
    This is to illustrate the "stop" command that can be use. It will send a 500us value which in most cases will stop the motor.
    If another value is needed, it can be changed in the library ESC.h under "ESC_STOP_PULSE" 

  27 April 2017
  by Eric Nantel
 */
#include "MyESC.h"
#define LED_PIN 2
#define ESC_PIN 4

MyESC myESC (ESC_PIN, 1000, 2000, 500);   // ESC_Name (ESC PIN, Minimum Value, Maximum Value, Arm Value)

void setup() {
  pinMode(LED_PIN, OUTPUT);       // LED Visual Output
  myESC.arm();                    // Send the Arm value so the ESC will be ready to take commands
  digitalWrite(LED_PIN, HIGH);    // LED High Once Armed
  delay(5000);                    // Wait for a while
}

void loop() {
  myESC.speed(1200);              // Set the speed to a testing value between 1000 and 2000
  delay(5000);                     // Wait for a while 
  myESC.speed(1400);              // Set the speed to a testing value between 1000 and 2000
  delay(5000);                     // Wait for a while 
  myESC.stop();                   // Stop the ESC altogether
  delay(5000);                    // Wait for a while until we restart  
}

