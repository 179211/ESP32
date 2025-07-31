const int PWM_CHANNEL = 0;    // ESP32 has 16 channels which can generate 16 independent waveforms
const int PWM_FREQ = 50;     // Recall that Arduino Uno is ~490 Hz. Official ESP32 example uses 5,000Hz
const int PWM_RESOLUTION = 10; // We'll use same resolution as Uno (8 bits, 0-255) but ESP32 can go up to 16 bits 

const int MAX_DUTY_CYCLE = (int)(pow(2, PWM_RESOLUTION) - 1); 
int x = (MAX_DUTY_CYCLE/20)*3;
int dutyCycle = 0;

const int LED_OUTPUT_PIN = 13;

const int DELAY_MS = 400;  // delay between fade increments

void setup() {
  Serial.begin(115200);
  delay(5000);

  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(LED_OUTPUT_PIN, PWM_CHANNEL);
}

void loop() {

  // fade up PWM on given channel
  for(dutyCycle = 0; dutyCycle <= x; dutyCycle++){   
    ledcWrite(PWM_CHANNEL, dutyCycle);
		Serial.println(dutyCycle);
    delay(DELAY_MS);
  }

  // fade down PWM on given channel
  for(dutyCycle = x; dutyCycle >= 0; dutyCycle--){
    ledcWrite(PWM_CHANNEL, dutyCycle);   
		Serial.println(dutyCycle);
    delay(DELAY_MS);
  }
}
