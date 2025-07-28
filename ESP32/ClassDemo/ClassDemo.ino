class ABC{
  public : 
    String Str;
    long num;

  ABC(){ }
  ABC(String s, long n){
    Str = s;
    num = n;
  }
  
  void GetFun2(long n){
    Serial.println("GetFun2 ...");
    num = n;
    Serial.println("GetFun2 Num: " + num);
  }

  void GetFun(){
    Serial.println("GetFun ...");
  }

  String GetStr(){ return Str;}
};

ABC obj1;
ABC obj2("STR", 10);

void setup() {
  Serial.begin(115200);

  obj1.Str = "ABC"; obj1.num = 20;
  
}

void loop() {
  obj1.GetFun();
  String s = obj2.GetStr();
}
