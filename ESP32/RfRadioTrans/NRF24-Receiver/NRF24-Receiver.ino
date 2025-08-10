/* To Arm ESC when its started beeping cont
1. Send Max 
2. Send Min after beep
3. Sent Test = Min + 2  after beep

To disArm / Not working
1. Send Max
2. Send Min 

We require this only in some cases 
*/

#include <ESP32Servo.h>
//#define LED_BUILTIN (2)

// #define ESC_0 32
// #define ESC_1 33
// const int PWMChannel_0 = 0;
// const int PWMChannel_1 = 1;

//Res: 8:  13 - 38
//Res: 10:  52 - 153
#define MIN_SPEED 56
#define MAX_SPEED 120
#define ARM_SPEED 52

/* Setting PWM properties */
const int PWMFreq = 50;
const int PWMResolution = 10;
const int MAX_DUTY_CYCLE = (int)(pow(2, PWMResolution) - 1);

int testPin = 1000;
const int NUM_Servo = 1;
// int servoPin[NUM_Servo] = {32, 33, 25, 26, 27, 14, 12, 13, 15, 4, 16, 17};
Servo myservo[NUM_Servo];  
int servoData[NUM_Servo][3] = {
  // {32, 500, 2000}, //  0
  // {33, 500, 2000}, //  1
  // {25, 500, 2000}, //  2
  // {26, 500, 2000}, //  3
  // {27, 500, 2000}, //  4
  // {14, 500, 2000}, //  5
  // {12, 500, 2000}, //  6
  // {13, 500, 2000}, //  7
  // {15, 500, 2000}, //  8
  {4,  1000, 2000} //  9
  // {16, 500, 2000}, // 10
  // {17, 500, 2000}  // 11
};

// struct MyServo {
//   Servo servo;
//   int pin;
//   int min = MIN_SPEED;
//   int max = MAX_SPEED;
// };
// MyServo s[NUM_Servo];

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

// MPU-6050
// I2C PIN: V.G.22.21
#define I2C_SCL 22
#define I2C_SDA 21

// GPS PIN: GTRV
#define Gps_Tx 1
#define Gps_Rx 3

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
  //servoArm(testPin);
  testAllServo();

  //delay(1000);
  //digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Started");
}

void loop() {
  myservo[0].write(40);
	// myservo[1].write(30);
	// myservo[2].write(40);
	// myservo[3].write(50);
	// myservo[4].write(60);
	// myservo[5].write(70);
	// myservo[6].write(80);
	// myservo[7].write(90);
	// myservo[8].write(100);
	// myservo[9].write(3); //4
	// myservo[10].write(120);
	// myservo[11].write(130);


  testServoMenu();
}

void testServoMenu(){
  Serial.print("testPin: ");
  Serial.println(testPin);
  if (Serial.available() > 0) { 
    int x = Serial.parseInt();
    Serial.print("Received: ");
    Serial.println(x);
    if (x > 0) testPin = x;
    Serial.println(testPin);
    // Do not use 0
    switch (testPin) {
        case 100:
            testAllServo();
            break;
        case 1 ... NUM_Servo:
            testServo(testPin - 1);
            //servoArm(testPin - 1);
            break;
        default:
            break;
    }

    testPin = 1000;

    /// if ledcAttach
    // testEsc(ESC_0);
    /// if ledcAttachPin
    // testEsc(PWMChannel_0);
    // testServo(testPin);
  }

}

void initServo(){
  Serial.println("initServo");

  // Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
  delay(500);    

  for (int i = 0; i < NUM_Servo; i++){
    Serial.println(i);
    myservo[i].setPeriodHertz(PWMFreq);
    myservo[i].attach(servoData[i][0], servoData[i][1], servoData[i][2]); //(PIN, MIN, MAX)
    delay(500);
    Serial.println(servoData[i][1]);
    myservo[i].writeMicroseconds(servoData[i][1]);

    delay(500);
    Serial.println(i);
  }
}

void testAllServo(){
  Serial.println("testAllServo");
  for (int i = 0; i < NUM_Servo; i++){
    Serial.println(i);
    testServo(i);
    delay(500);
    Serial.println(i);
  }
}

void servoArm(int x){
  Serial.println(servoData[x][2]);
  myservo[x].writeMicroseconds(servoData[x][2]);
  delay(8000);
  Serial.println(servoData[x][1]);
  myservo[x].writeMicroseconds(servoData[x][1]);
  delay(8000);
  Serial.println(servoData[x][1]);
  myservo[x].writeMicroseconds(servoData[x][1] + 200);
  delay(8000);
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
  ledcWrite(pin, MIN_SPEED);

  // ledcSetup(channel, PWMFreq, PWMResolution);
  // ledcAttachPin(pin, channel);
  // EscArm(channel);
  delay(500);
}

void EscArm(uint8_t x){
  ledcWrite(x, MAX_SPEED);
  delay(8000);    
  ledcWrite(x, MIN_SPEED);
  delay(8000);    
  ledcWrite(x, MIN_SPEED + 2);
  delay(500);    
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

