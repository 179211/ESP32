bool swList[8] = {1, 1, 1, 1, 1, 1, 1, 1};
byte swNo=0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Started");
  Serial.println("*************************************");

  swNo=255;
  decodeBtn();
  encodeBtn();
  swNo=205;
  decodeBtn();
  encodeBtn();
}

void loop() {
  // put your main code here, to run repeatedly:

}


void encodeBtn(){
  swNo = 0;
  swNo += swList[0] << 7;
  swNo += swList[1] << 6;
  swNo += swList[2] << 5;
  swNo += swList[3] << 4;
  swNo += swList[4] << 3;
  swNo += swList[5] << 2;
  swNo += swList[6] << 1;
  swNo += swList[7] << 0;
  Serial.println("swNo ");
  Serial.println(swNo);
}

void decodeBtn(){
  byte recByte = swNo;  
  swNo = 0;
  swList[0] = recByte >> 7;
  swNo += swList[0]*128;
  swList[1] = (recByte-swNo) >> 6;
  swNo += swList[1]*64;
  swList[2] = (recByte-swNo) >> 5;
  swNo += swList[2]*32;
  swList[3] = (recByte-swNo) >> 4;
  swNo += swList[3]*16;
  swList[4] = (recByte-swNo) >> 3;
  swNo += swList[4]*8;
  swList[5] = (recByte-swNo) >> 2;
  swNo += swList[5]*4;
  swList[6] = (recByte-swNo) >> 1;
  swNo += swList[6]*2;
  swList[7] = (recByte-swNo) >> 0;
  swNo += swList[7]*0;
  Serial.println("swList ");
  for(int i=0; i<8; i++){
    Serial.print(swList[i]);
  }
  Serial.println("");
}
