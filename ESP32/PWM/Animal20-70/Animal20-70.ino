#define BUZZER_PIN 13
#define BUZZER_CHANNEL 0

/************************************************
Species	Approximate Range (Hz) 
human	64-23,000

Cow
23 Hz to 37 kHz

Dog
47,000 to 65,000 Hz
************************************************/

int freqLow = 24000;
int freqHigh = 60000;
int freq = freqLow;
int resolution = 4;
uint32_t mDuty = 0; 
int dutyCycle = 0;

void setup() {
  Serial.begin(115200); 
  delay(100);
  Serial.println ("*************************************");

  mDuty = (pow(2, resolution)) / 2;
  Serial.println(mDuty); 
  ledcSetup(BUZZER_CHANNEL, freq, resolution);
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL); 
}
 
void loop() { 
    freq = freq + 1000;
    Serial.println(freq); 
    ledcWriteTone(BUZZER_CHANNEL, freq);
    delay(1000);
    ledcWrite(BUZZER_CHANNEL, 0);
    delay(100);
    if (freq == freqHigh){
      freq = freqLow;  
    }
}
