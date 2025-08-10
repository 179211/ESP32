/* To Arm ESC when its started beeping cont
1. Send Max 
2. Send Min after beep
3. Sent Test = Min + 2  after beep

To disArm / Not working
1. Send Max
2. Send Min 

We require this only in some cases 
*/

#define ESC_PIN 4 
#define LED_BUILTIN 2
#define POT_PIN 15

//#define MIN_SPEED 1000 // speed just slow enough to turn motor off
//#define MAX_SPEED 1240 // 2000 speed where my motor drew 3.6 amps at 12v.
//#define ARM_SPEED 500

//Res: 8:  13 - 38
//Res: 10:  52 - 153
#define MIN_SPEED 56
#define MAX_SPEED 120
#define ARM_SPEED 56

long int val; // variable to read the value from the analog pin
int dutyCycle = 15;
int testPin = 0;

/* Setting PWM properties */
float dCycle =0.1;
bool b;
const int PWMFreq = 50;
const int PWMChannel_0 = 0;
const int PWMResolution = 10;
const int MAX_DUTY_CYCLE = (int)(pow(2, PWMResolution) - 1);


void setup() {
  Serial.begin(115200);
  delay(5000);
  Serial.println("Hi Test");

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(POT_PIN, INPUT);
  pinMode(ESC_PIN, OUTPUT);
  delay(1000);

	ledcAttachChannel(ESC_PIN, PWMFreq, PWMResolution, PWMChannel_0);

  // ledcSetup(PWMChannel_0, PWMFreq, PWMResolution);
  // ledcAttachPin(ESC_PIN, PWMChannel_0);

  Serial.print("PWMResolution ");
  Serial.println(PWMResolution);
  Serial.print("MAX_DUTY_CYCLE ");
  Serial.println(MAX_DUTY_CYCLE);
  Serial.print("MIN ");
  Serial.println(MAX_DUTY_CYCLE/20);
  Serial.print("MAX ");
  Serial.println((MAX_DUTY_CYCLE/20)*2);
  delay(5000);
    
  //EspArm(ESC_PIN);
  //EspArm(PWMChannel_0);

  digitalWrite(LED_BUILTIN, HIGH);          // LED High Once Armed
  delay(5000);                          // Wait for a while
}

void loop() {
  // val = analogRead(POT_PIN);            // reads the value of the potentiometer (value between 0 and 4095)
  // val = map(val, 0, 4095, MIN_SPEED, MAX_SPEED);  // scale it to use it with the ESC (value between 1000 and 2000)
  // Serial.println(val);
  //ledcWriteChannel(PWMChannel_0, val);
  
  //ledcWrite(ESC_PIN, MIN_SPEED);
  //ledcWriteChannel(PWMChannel_0, MIN_SPEED);
  delay(500);

  if (Serial.available() > 0) { 
    testPin = Serial.parseInt();
    Serial.print("Received: ");
    Serial.println(testPin);

    /// ESC Arm
    if (testPin == 1){
      ledcWriteChannel(PWMChannel_0, MAX_SPEED);
      Serial.println("MAX_SPEED");
      delay(5000);
    }
    else if (testPin == 2){
      ledcWriteChannel(PWMChannel_0, MIN_SPEED);
      Serial.println("MIN_SPEED");
      delay(5000);
    }
    else if (testPin == 3){
      ledcWriteChannel(PWMChannel_0, MIN_SPEED + 2);
      Serial.println("MIN_SPEED + 2");
      delay(5000);
    }
  }

}

void EspArm(uint8_t x){
  ledcWriteChannel(x, MAX_SPEED);
  delay(5000);    
  ledcWriteChannel(x, MIN_SPEED);
  delay(5000);    
  ledcWriteChannel(x, MIN_SPEED + 2);
  delay(500);    
}