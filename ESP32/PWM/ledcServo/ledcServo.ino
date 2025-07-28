#include "soc/soc_caps.h"
#include <string>

/* ESP32 Servo Sweep */
const int servoPin = 4;  /* GPIO16 */
int potpin = 15;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin

int dutyCycle = 15;

/* Setting PWM properties */
float dCycle =0.1;
bool b;
const int PWMFreq = 50;
const int PWMChannel = 0;
const int PWMResolution = 10;
const int MAX_DUTY_CYCLE = (int)(pow(2, PWMResolution) - 1);

void setup()
{  
  Serial.begin(115200);
  delay(5000);
  Serial.println("Hi Test");
  Serial.println(MAX_DUTY_CYCLE);

  pinMode(potpin, INPUT);
  pinMode(servoPin, OUTPUT);
  b = ledcAttach(servoPin, PWMFreq, PWMResolution);

  Serial.print("PWMResolution ");
  Serial.println(PWMResolution);

  for(dutyCycle = 1; dCycle <= 2; dutyCycle++)
  {
    dCycle = map(dutyCycle, 0, MAX_DUTY_CYCLE, 0.0, 20.0); // Convert the analog read value from 0 to 1023 into a BYTE value from 0 to 255
    Serial.print(dutyCycle);
    Serial.print("-----------");
    Serial.println(dCycle);
  }

  //Res: 8:  13 - 38
  //Res: 10:  52 - 153
  ledcWrite(servoPin, 54);
  delay(7000);    
  ledcWrite(servoPin, 56);
  delay(7000);    
  ledcWrite(servoPin, 54);
  delay(7000);    

}
void loop()
{

  //ledcWrite(servoPin, 54);

  for(dutyCycle = 52; dutyCycle <= 132; dutyCycle++)
  {
    Serial.println(dutyCycle);
    ledcWrite(servoPin, dutyCycle);
    delay(700);    
  }
  for(dutyCycle = 132; dutyCycle >= 52; dutyCycle--)
  {
    Serial.println(dutyCycle);
    ledcWrite(servoPin, dutyCycle);
    delay(700);    
  }

  // val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
  // Serial.println(val);
  // val = map(val, 0, 4095, 0, 255);     // scale it for use with the servo (value between 0 and 180)
  // val = map(val, 0, 255, 52, 132); // Map the receiving value form 0 to 255 to 0 1000 to 2000, values u
  // ledcWrite(servoPin, val);
  // Serial.println(val);
 
}