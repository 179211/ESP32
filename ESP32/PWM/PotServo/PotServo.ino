const int SERVO_PIN = 15;
const int POT_PIN = 4;
int PWM_CHANNEL = 0;
int PWM_FREQUENCY = 50;
int PWM_RESOLUTION = std::min(16, SOC_LEDC_TIMER_BIT_WIDE_NUM);
const int MAX_DUTY_CYCLE = (int)(pow(2, PWM_RESOLUTION) - 1); 
int DEFAULT_MIN_PULSE_WIDTH_US = 544;   // the shortest pulse sent to a servo
int DEFAULT_MAX_PULSE_WIDTH_US = 2400;  // the longest pulse sent to a servo


void setup() {
  initSerial();
  
  ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(SERVO_PIN, PWM_CHANNEL);
  Serial.println(MAX_DUTY_CYCLE);

}

void loop() {
  ledcWrite(PWM_CHANNEL, 400);
    for(int posDegrees = 0; posDegrees <= 3000; posDegrees++) {
      ledcWrite(PWM_CHANNEL, posDegrees);
      delay(20);
    }

    for(int posDegrees = 3000; posDegrees >= 0; posDegrees--) {
      ledcWrite(PWM_CHANNEL, posDegrees);
      delay(20);
    }
}
void initSerial(){
  Serial.begin(115200);
  delay(2000);
  Serial.println ("Started");
  Serial.println ("*************************************");
}
