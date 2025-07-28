const int PWM_CHANNEL = 0;    // ESP32 has 16 channels which can generate 16 independent waveforms
const int PWM_FREQ = 500;     // Recall that Arduino Uno is ~490 Hz. Official ESP32 example uses 5,000Hz
const int PWM_RESOLUTION = 8; // We'll use same resolution as Uno (8 bits, 0-255) but ESP32 can go up to 16 bits 

// The max duty cycle value based on PWM resolution (will be 255 if resolution is 8 bits)
const int MAX_DUTY_CYCLE = (int)(pow(2, PWM_RESOLUTION) - 1); 

const int LED_OUTPUT_PIN = 19;
const int POT_PIN = 4;

const int DELAY_MS = 100;  // delay between fade increments

void setup() {
  initSerial();
  
  // Sets up a channel (0-15), a PWM duty cycle frequency, and a PWM resolution (1 - 16 bits) 
  // ledcSetup(uint8_t channel, double freq, uint8_t resolution_bits);
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  // ledcAttachPin(uint8_t pin, uint8_t channel);
  ledcAttachPin(LED_OUTPUT_PIN, PWM_CHANNEL);
}

void loop() {
  int dutyCycle = analogRead(POT_PIN);
  //Serial.println(dutyCycle);
  //delay(1000);
  dutyCycle = map(dutyCycle, 0, 4095, 0, MAX_DUTY_CYCLE);
  Serial.println(dutyCycle);
  //delay(1000);
 
  ledcWrite(PWM_CHANNEL, dutyCycle);
  delay(DELAY_MS);
}

void initSerial(){
  Serial.begin(115200);
  delay(2000);
  Serial.println ("Started");
  Serial.println ("*************************************");
}
