#include <RCSwitch.h>

#define PIN_RcSend 17
#define PIN_RcRec 16

TaskHandle_t Task1;
//RCSwitch mySwitch = RCSwitch();
RCSwitch mySwitchRec = RCSwitch();

void setup() {
  initDisplay();
  GetCore();
  
  //mySwitch.enableTransmit(PIN_RcSend);

  createReceiver();
  //delay(50000); 
}

void loop() {
    //Serial.println(xPortGetCoreID());
    //loopSend();
    //receiveRfCode2();
}

void createReceiver(){
  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    receiveRfCode,   /* Task function. */
                    "receiveRfCode",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 
}

//Task1code: Receive RF code
void receiveRfCode( void * pvParameters ){
  mySwitchRec.enableReceive(digitalPinToInterrupt(PIN_RcRec));
  for(;;){
    receiveRfCode2();
  } 
}

void receiveRfCode2(){
  //Serial.println(xPortGetCoreID());
  if (mySwitchRec.available()) {
    Serial.print("Value: ");
    Serial.print( mySwitchRec.getReceivedValue() );
    Serial.print(" / ");
    Serial.print("Bitlength: ");
    Serial.print( mySwitchRec.getReceivedBitlength() );
    Serial.print("bit ");
    Serial.print(" / ");
    Serial.print("Protocol: ");
    Serial.println( mySwitchRec.getReceivedProtocol() );
    
    mySwitchRec.resetAvailable();
  }
}

void GetCore(){
  Serial.print("setup() running on core ");
  Serial.println(xPortGetCoreID());
}

void initDisplay(){
  Serial.begin(115200);
  delay(2000);
  Serial.println ("Started");
  Serial.println ("*************************************");
}

void loopSend() {
  /* Same switch as above, but using binary code */
  //mySwitch.send("000000000001010100010001");
  delay(1000);  
  //mySwitch.send("000000000001010100010100");
  delay(1000);

  Serial.println ("Data Send");
}
