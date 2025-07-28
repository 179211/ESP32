struct Button {
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  uint32_t lastTrigger;
};

Button button1 = {23, 0, 0};

void IRAM_ATTR button1Intp() {
  if (millis() - button1.lastTrigger > 500) {
    button1.lastTrigger = millis();
    button1.numberKeyPresses += 1;
    Serial.printf("Button 1 has been pressed %u times\n", button1.numberKeyPresses);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println ("Started");
  Serial.println ("*************************************");
  
  pinMode(button1.PIN, INPUT_PULLUP);
  attachInterrupt(button1.PIN, button1Intp, FALLING);
}

void loop() {
}
