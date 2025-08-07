#include <ESP32Servo.h>

// 16 servo objects can be created on the ESP32
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 

int OnBoardLed = 2;
int i = 0;
int pos = 0;

#define NUM_Servo  10
int servoPin[NUM_Servo] = {4, 15, 13, 32, 33, 25, 26, 27, 16, 17};
Servo myservo[NUM_Servo];  

void setup() {
  Serial.begin(115200);
  delay(5000);

	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

  for (i=0; i < NUM_Servo; i++){
    myservo[i].setPeriodHertz(50);
    myservo[i].attach(servoPin[i], 500, 2000); //(PIN, MIN, MAX)
  }
	// using default min/max of 1000us and 2000us
	// different servos may require different min/max settings
	// for an accurate 0 to 180 sweep
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

	for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
        Serial.println(pos);
		myservo[0].write(pos);    // tell servo to go to position in variable 'pos'
		delay(15);             // waits 15ms for the servo to reach the position
	}
	for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
        Serial.println(pos);
		myservo[0].write(pos);    // tell servo to go to position in variable 'pos'
		delay(15);             // waits 15ms for the servo to reach the position
	}
}

