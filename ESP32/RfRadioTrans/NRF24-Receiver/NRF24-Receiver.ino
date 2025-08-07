#include <ESP32Servo.h>
//#define LED_BUILTIN (2)

#define ESC_0 32
#define ESC_1 33
const int PWMChannel_0 = 0;
const int PWMChannel_1 = 1;

//Res: 8:  13 - 38
//Res: 10:  52 - 153
#define MIN_SPEED 56
#define MAX_SPEED 120
#define ARM_SPEED 52

/* Setting PWM properties */
const int PWMFreq = 50;
const int PWMResolution = 10;
const int MAX_DUTY_CYCLE = (int)(pow(2, PWMResolution) - 1);

const int NUM_Servo = 12;
int testPin = 0;
int servoPin[NUM_Servo] = {32, 33, 25, 26, 27, 14, 12, 13, 15, 4, 16, 17};
Servo myservo[NUM_Servo];  

#define ChipEnable 2
#define PIN_V_MO 23
#define PIN_V_MI 19
#define PIN_V_CLK 18
#define PIN_V_CSN 5
#define PIN_V_CE 2

// Separate Pins to connect but not initiated
// #define PIN_H_MO 13
// #define PIN_H_MI 12
// #define PIN_H_CLK 14
// #define PIN_H_CSN 15
// #define PIN_H_CE 4

// I2C PIN: V.G.22.21
#define I2C_SCL 22
#define I2C_SDA 21

// GPS PIN: GTRV
#define Gps_Tx 1
#define Gps_Rx 3

// MPU-6050
// Pin pending


int i = 0;
int pos = 0;

void setup() {
  Serial.begin(115200); while (!Serial) {}
  Serial.println("Hi Test");
  // pinMode(LED_BUILTIN, OUTPUT);

  // initEsc(ESC_0, PWMChannel_0);
  // initEsc(ESC_1, PWMChannel_1);
  // Serial.println("initEsc");

  initServo();
  Serial.println("initServo");

  delay(1000);
  //digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Started");
}

void loop() {
	myservo[0].write(20);
	myservo[1].write(30);
	myservo[2].write(40);
	myservo[3].write(50);
	myservo[4].write(60);
	myservo[5].write(70);
	myservo[6].write(80);
	myservo[7].write(90);
	myservo[8].write(100);
	myservo[9].write(110);
	myservo[10].write(120);
	myservo[11].write(130);

  //testEsc(PWMChannel_0);
  //testEsc(ESC_1);

  if (Serial.available() > 0) { 
    testPin = Serial.parseInt();
    Serial.print("Received: ");
    Serial.println(testPin);
  }
  testServo(testPin);
}

void testEsc(uint8_t x){
  Serial.print("test ESC Started ");
  Serial.println(x);
  for (i = MIN_SPEED; i < MAX_SPEED; i++){
    ledcWrite(x, i);
 		delay(15);
 }
  for (i = MAX_SPEED; i > MIN_SPEED; i--){
    ledcWrite(x, i);
		delay(15);
  }
  Serial.print("test ESC Ended ");
  Serial.println(x);
}

void testServo(int servoNo){
  Serial.print("testServo Started ");
  Serial.println(servoNo);
	for (pos = 0; pos <= 180; pos += 1) {
		myservo[servoNo].write(pos);
		delay(15);
	}
	for (pos = 180; pos >= 0; pos -= 1) {
		myservo[servoNo].write(pos);
		delay(15);
	}
  Serial.println("testServo Ended");
}

void initEsc(uint8_t pin, uint8_t channel){
  pinMode(pin, OUTPUT);
	ledcAttach(pin, PWMFreq, PWMResolution);
  EspArm(pin);
  // ledcSetup(channel, PWMFreq, PWMResolution);
  // ledcAttachPin(pin, channel);
  // EspArm(channel);
  delay(500);
}

void EspArm(uint8_t x){
  ledcWrite(x, ARM_SPEED);
  delay(500);    
  ledcWrite(x, ARM_SPEED + 1);
  delay(500);    
  ledcWrite(x, ARM_SPEED);
  delay(500);    
}

void initServo(){
  	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
  delay(500);    

  for (int i = 0; i < NUM_Servo; i++){
    myservo[i].setPeriodHertz(PWMFreq);
    myservo[i].attach(servoPin[i], 500, 2000); //(PIN, MIN, MAX)
    delay(100);
  }
}
