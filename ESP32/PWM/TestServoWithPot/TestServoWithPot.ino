#include "ESC.h"
#define ESC_PIN (16) // connected to ESC control wire
#define LED_BUILTIN (2) // not defaulted properly for ESP32s/you must define it
#define POT_PIN (4) // Analog pin used to connect the potentiometer center pin

//#define MIN_SPEED 1000 // speed just slow enough to turn motor off
//#define MAX_SPEED 1240 // 2000 speed where my motor drew 3.6 amps at 12v.
//#define ARM_SPEED 500

#define MIN_SPEED 57 // speed just slow enough to turn motor off
#define MAX_SPEED 120 // 2000 speed where my motor drew 3.6 amps at 12v.
#define ARM_SPEED 52

//ESC myESC (ESC_PIN, MIN_SPEED, MAX_SPEED, ARM_SPEED);         // ESC_Name (PIN, Minimum Value, Maximum Value, Arm Value)

long int val; // variable to read the value from the analog pin
int dutyCycle = 15;

/* Setting PWM properties */
float dCycle =0.1;
bool b;
const int PWMFreq = 50;
const int PWMChannel = 0;
const int PWMResolution = 10;
const int MAX_DUTY_CYCLE = (int)(pow(2, PWMResolution) - 1);


void setup() {
  Serial.begin(115200);
  delay(5000);
  Serial.println("Hi Test");

  pinMode(LED_BUILTIN, OUTPUT);             // LED Visual Output
  pinMode(POT_PIN, INPUT);
  pinMode(ESC_PIN, OUTPUT);
  delay(5000);

  //myESC.arm();                          // Send the Arm value
  b = ledcAttach(ESC_PIN, PWMFreq, PWMResolution);
  Serial.print("PWMResolution ");
  Serial.println(PWMResolution);
  Serial.print("MAX_DUTY_CYCLE ");
  Serial.println(MAX_DUTY_CYCLE);
  delay(5000);
    
  EspArm();

  digitalWrite(LED_BUILTIN, HIGH);          // LED High Once Armed
  delay(5000);                          // Wait for a while
}

void loop() {
  val = analogRead(POT_PIN);            // reads the value of the potentiometer (value between 0 and 4095)
  val = map(val, 0, 4095, MIN_SPEED, MAX_SPEED);  // scale it to use it with the ESC (value between 1000 and 2000)
  Serial.println(val);
  ledcWrite(ESC_PIN, val);
  delay(700);    

}


void EspArm(){
  //Res: 8:  13 - 38
  //Res: 10:  52 - 153
  ledcWrite(ESC_PIN, ARM_SPEED);
  delay(700);    
  ledcWrite(ESC_PIN, ARM_SPEED + 2);
  delay(700);    
  ledcWrite(ESC_PIN, ARM_SPEED);
  delay(700);    
}