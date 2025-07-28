/*
  Simple example for receiving

  https://github.com/sui77/rc-switch/
*/

#define BUZZER_PIN 13
#define PIR_PIN 27
#define RF_PIN 4
#define BuzzDuration 1000
#define LongBuzzDuration 5000

#include <RCSwitch.h>

int ReceivedValue;
int TouchValue = 2047;

unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startBuzzer = false;
boolean motion = false;

RCSwitch mySwitch = RCSwitch();

void setup() {
  //Serial.begin(9600);
  Serial.begin(115200);
  delay(2000);
  Serial.print("Started");
  mySwitch.enableReceive(digitalPinToInterrupt(RF_PIN));
  //mySwitch.enableReceive(RF_PIN);  // Arduino

  pinMode(PIR_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.print("Started");
}

void loop() {
  fetchRF();
  fetchPIR();
}

void fetchPIR(){
}

void fetchRF(){
  if (mySwitch.available()) {
    ReceivedValue = mySwitch.getReceivedValue();
    if ( TouchValue == ReceivedValue){
      Serial.print("Value: ");
      Serial.print( ReceivedValue );

      playLongBuzzer();

      Serial.println( "End Touch" );
    }

    mySwitch.resetAvailable();
  }
}

void playLongBuzzer(){
      digitalWrite(BUZZER_PIN, HIGH);      
      delay(LongBuzzDuration);
      digitalWrite(BUZZER_PIN, LOW);
}
void playBuzzer(){
      digitalWrite(BUZZER_PIN, HIGH);
      delay(BuzzDuration);
      digitalWrite(BUZZER_PIN, LOW);
}
