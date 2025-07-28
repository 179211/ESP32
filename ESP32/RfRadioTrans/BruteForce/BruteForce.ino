#include <bitset>

using namespace std;

const int bitLength = 12;
int smallNumber = 0;
int maxNumber = (1 << bitLength); // 2^120

unsigned long endTime = 0;
unsigned long startTime = 0;
int bruteCount = 0;
bool stopBF = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(2000);

  // bruteForce(10, 20);
  // bruteForce(20, 10);
}

void loop() {

  maxNumber = bruteForce(smallNumber, maxNumber);
  stopBF = true;

    // for (int i = 0; i < maxNumber; ++i){
    //   String bitString = "xyz";
    //   bitset<bitLength> binary(i);
    //   bitString = binary.to_string().c_str();
    //   Serial.println(bitString);
    //   delay(100);
    // }
}


int bruteForce(int startNo, int endNo){
  startTime = millis();
  bruteCount++;

  int dir = 1;
  int i = startNo;
  int limit = endNo;
  if (startNo > endNo){
    dir = -1;
  }

  while (startNo != endNo && stopBF == false)
  {
    String bitString = "";
    bitset<bitLength> binary(startNo);
    bitString = binary.to_string().c_str();
    Serial.println(bitString);
    delay(100);

    startNo = startNo + dir;
  }

  endTime = millis();
  Serial.print("EndTime - ");
  Serial.println(endTime - startTime);
  return startNo;  
}