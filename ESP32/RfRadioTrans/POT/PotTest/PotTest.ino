// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6) 
const int potPin = 13;
const int potRx = 34; //1955-60 4095
const int potRy = 35;
const int potLx = 36;
const int potLy = 39;
const int potL = 27;
const int potR = 13;

// variable for storing the potentiometer value
int potValue = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  // Reading potentiometer value
  potValue = analogRead(potPin);
  Serial.println(potValue);
  delay(500);
}
