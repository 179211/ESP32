int freq = 500;
int ledChannel = 0;
int resolution = 4;
uint32_t mDuty = 0; 
int dutyCycle = 0;

void setup() {
  Serial.begin(115200); 
  delay(100);
  Serial.println ("*************************************");

  mDuty = (pow(2, resolution)) / 2;
  Serial.println(mDuty); 
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(4, ledChannel);
 
}
 
void loop() { 
  for (dutyCycle = 0; dutyCycle <= mDuty; dutyCycle++) {
    Serial.println(dutyCycle); 
    ledcWrite(ledChannel, dutyCycle);
    delay(500);
  }
  for (dutyCycle = mDuty; dutyCycle >= 0; dutyCycle--) {
    Serial.println(dutyCycle); 
    ledcWrite(ledChannel, dutyCycle);
    delay(500);
  } 
}
