int digitalPin = 7;   // KY-037 digital interface
int analogPin = 15;   // KY-037 analog interface
int ledPin = 2;      // Arduino LED pin
int digitalVal;       // digital readings
int analogVal;        // analog readings

void setup()
{
  initSerial();

  pinMode(digitalPin,INPUT); 
  pinMode(analogPin, INPUT);
  pinMode(ledPin,OUTPUT);      
}

void loop()
{
  // Read the digital inteface
  digitalVal = digitalRead(digitalPin); 
  
  if(digitalVal == HIGH) 
  {
    digitalWrite(ledPin, HIGH); // Turn ON Arduino's LED
  }
  else
  {
    digitalWrite(ledPin, LOW);  // Turn OFF Arduino's LED
  }

  // Read analog interface
  analogVal = analogRead(analogPin);
  // Print analog value to serial
  Serial.println(analogVal);  
}

void initSerial(){
  Serial.begin(115200);
  delay(2000);
  Serial.println ("Started");
  Serial.println ("*************************************");
}
