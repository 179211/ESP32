#define ESC_PIN (13) // connected to ESC control wire
#define LED_BUILTIN (2) // not defaulted properly for ESP32s/you must define it
#define POT_PIN (4) // Analog pin used to connect the potentiometer center pin

//#define MIN_SPEED 1000 // speed just slow enough to turn motor off
//#define MAX_SPEED 1240 // 2000 speed where my motor drew 3.6 amps at 12v.
//#define ARM_SPEED 500

//Res: 8:  13 - 38
//Res: 10:  52 - 153
#define MIN_SPEED 26
#define MAX_SPEED 132

long int val; // variable to read the value from the analog pin
int dutyCycle = 15;

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

  ledcSetup(PWMChannel_0, PWMFreq, PWMResolution);
  ledcAttachPin(ESC_PIN, PWMChannel_0);

  Serial.print("PWMResolution ");
  Serial.println(PWMResolution);
  Serial.print("MAX_DUTY_CYCLE ");
  Serial.println(MAX_DUTY_CYCLE);
  Serial.print("MIN ");
  Serial.println(MAX_DUTY_CYCLE/20);
  Serial.print("MAX ");
  Serial.println((MAX_DUTY_CYCLE/20)*2);
  delay(5000);
    
  ledcWrite(PWMChannel_0, MIN_SPEED);
  delay(500);
  ledcWrite(PWMChannel_0, MIN_SPEED+1);
  delay(500);
  ledcWrite(PWMChannel_0, MIN_SPEED);
  delay(500);

  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
}

void loop() {
  for(int i = MIN_SPEED; i < MAX_SPEED; i++ ){
    Serial.println(i);
    ledcWrite(PWMChannel_0, i);
    delay(500);
  }
  for(int i = MAX_SPEED; i > MIN_SPEED; i-- ){
    Serial.println(i);
    ledcWrite(PWMChannel_0, i);
    delay(500);
  }
}
