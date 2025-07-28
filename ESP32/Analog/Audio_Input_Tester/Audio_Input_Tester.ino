void setup() {
  Serial.begin(115200);
  delay(1000); 
}

void loop() {
  Serial.println(analogRead(A0));
}
