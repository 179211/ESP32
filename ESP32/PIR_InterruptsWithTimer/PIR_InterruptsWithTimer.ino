/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

struct Button {
  const uint8_t PIN;
  String Name;
  uint32_t timeSeconds;
  unsigned long lastTrigger;
  boolean startTimer;
};

Button button1 = {18, "Led 2", 5, 0, false};

const int led = 2;
unsigned long now = millis();

void IRAM_ATTR IntpButton1() {
  if (millis() - button1.lastTrigger > 500) {
    button1.lastTrigger = millis();
    Serial.println("Button Pressed !!!");
    digitalWrite(led, HIGH);
    button1.startTimer = true;
  }
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  pinMode(button1.PIN, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  
  // LOW, HIGH, CHANGE, FALLING, RISING
  attachInterrupt(button1.PIN, IntpButton1, RISING); // RISING for PIR, FALLING for button
}

void loop() {
  // Current time
  now = millis();

  // Turn off the LED after the number of seconds defined in the timeSeconds variable
  if(button1.startTimer && (now - button1.lastTrigger > (button1.timeSeconds*1000))) {
    Serial.println("Time Expired ...");
    digitalWrite(led, LOW);
    button1.startTimer = false;
  }
}
