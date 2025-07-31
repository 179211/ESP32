#define PWM_PIN (13) // connected to ESC control wire
#define LED_BUILTIN (2) // not defaulted properly for ESP32s/you must define it
#define POT_PIN (4) // Analog pin used to connect the potentiometer center pin

//#define MIN_SPEED 1000 // speed just slow enough to turn motor off
//#define MAX_SPEED 1240 // 2000 speed where my motor drew 3.6 amps at 12v.
//#define ARM_SPEED 500

//Res: 8  :       :  13 - 38
//Res: 10 : 1023  :  52 - 153
#define MIN_SPEED 50
#define MAX_SPEED 120
#define ARM_SPEED 50

long int val; // variable to read the value from the analog pin

/* Setting PWM properties */
const int PWMFreq = 50;
const int PWMResolution = 10;
const int MAX_DUTY_CYCLE = (int)(pow(2, PWMResolution) - 1);
const int MIN_LIMIT = MAX_DUTY_CYCLE/20;
const int PWMChannel_0 = 0;


void setup() {
  Serial.begin(115200);
  delay(5000);
  Serial.println("Hi Test");

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(POT_PIN, INPUT);
  pinMode(PWM_PIN, OUTPUT);
  delay(1000);

  ledcSetup(PWMChannel_0, PWMFreq, PWMResolution);
  ledcAttachPin(PWM_PIN, PWMChannel_0);

  Serial.print("PWMResolution ");
  Serial.println(PWMResolution);
  Serial.print("MAX_DUTY_CYCLE ");
  Serial.println(MAX_DUTY_CYCLE);
  Serial.print("MIN ");
  Serial.println(MIN_LIMIT);
  Serial.print("MAX ");
  Serial.println(MIN_LIMIT*2);
  delay(5000);
    
  EspArm();

  digitalWrite(LED_BUILTIN, HIGH);          // LED High Once Armed
  delay(5000);                          // Wait for a while
}

void loop() {
  val = analogRead(POT_PIN);            // reads the value of the potentiometer (value between 0 and 4095)
  val = map(val, 0, 4095, MIN_SPEED, MAX_SPEED);  // scale it to use it with the ESC (value between 1000 and 2000)
  Serial.println(val);
  ledcWrite(PWM_PIN, val);
  delay(500);

}

void EspArm(){
  ledcWrite(PWM_PIN, ARM_SPEED);
  delay(500);    
  ledcWrite(PWM_PIN, ARM_SPEED + 2);
  delay(500);    
  ledcWrite(PWM_PIN, ARM_SPEED);
  delay(500);    
}