int sensorPin = 36;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup() {
  Serial.begin(9600);
  delay(2000);
  Serial.println ("Started");
  Serial.println ("*************************************");  
}

void loop() {
  sensorValue = analogRead(sensorPin);
  //int sensorValue2 = map(analogRead(sensorPin), 0, 1023, 0, 255);
  int sensorValue2 = map(analogRead(sensorPin), 0, 4095, 0, 255);
  Serial.println (sensorValue);
  Serial.println (sensorValue2);
  
}
