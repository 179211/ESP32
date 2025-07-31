#include <ESP32Servo.h>

// 16 servo objects can be created on the ESP32
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
#define PWM_PIN (13) 
#define LED_BUILTIN (2)
#define POT_PIN (4)

int i = 0;
int val = 0;
int pos = 0;
int min_limit = 800;
int max_limit = 2400;

#define NUM_Servo  1
int servoPin[NUM_Servo] = {13};
Servo myservo[NUM_Servo];  

void setup() {
  Serial.begin(115200);
  delay(5000);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(POT_PIN, INPUT);
  delay(1000);

	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

  for (i=0; i < NUM_Servo; i++){
    myservo[i].setPeriodHertz(50);
    myservo[i].attach(servoPin[i], min_limit, max_limit); //(PIN, MIN, MAX)
  }

	myservo[0].write(pos);
	delay(150);
	myservo[0].write(pos+1);
	delay(150);
	myservo[0].write(pos);
	delay(150);

  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
}

void loop() {
  val = analogRead(POT_PIN);            // reads the value of the potentiometer (value between 0 and 4095)
  pos = map(val, 0, 4095, 0, 180);  // scale it to use it with the ESC (value between 1000 and 2000)
  Serial.println(pos);
	myservo[0].write(pos);
}

