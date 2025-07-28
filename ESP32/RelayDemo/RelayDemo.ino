#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>

/*
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
*/


/*Put your SSID & Password*/
const char* ssid = "_Naruto_";  // Enter SSID here
const char* password = "WakeUpNow@5";  //Enter Password here
WebServer server(80);

#define NUM_RELAYS  5
int relayGPIOs[NUM_RELAYS] = {2, 26, 27, 25, 33};

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP()); 
  Serial.print("Got MAC: ");  Serial.println(WiFi.macAddress());
  Serial.println("To list connected devices: cmd > arp -a");

  for(int i=1; i<=NUM_RELAYS; i++){
    pinMode(relayGPIOs[i-1], OUTPUT);
    digitalWrite(relayGPIOs[i-1], LOW);
  }

  server.on("/", handle_OnConnect);
  server.on("/update", handle_update);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handle_OnConnect() {
  Serial.println("handle_OnConnect");
  server.send(200, "text/html", SendHTML()); 
}

void handle_update() {
    String relay;
    String state;
    // Send a GET request to <ESP_IP>/update?relay=1&state=1
    if (server.hasArg("relay") & server.hasArg("state")) {
      relay = server.arg("relay") ;
      state = server.arg("state") ;
      digitalWrite(relayGPIOs[relay.toInt()-1], state.toInt());
    }
    else {
      Serial.println("Invalid message sent");
    }
    Serial.println(relay + state);
    
  server.send(200, "text/html", SendHTML()); 
}

void handle_NotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}

String relayState(int numRelay){
  if(digitalRead(relayGPIOs[numRelay-1])){
    return "checked";
  }
  else {
    return "";
  }
}

String GenButton(){
  String buttons ="";
  for(int i=1; i<=NUM_RELAYS; i++){
    String relayStateValue = relayState(i);
    buttons+= "<h4>Relay #" + String(i) + " - GPIO " + relayGPIOs[i-1] + "</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"" + String(i) + "\" "+ relayStateValue +"><span class=\"slider\"></span></label>";
  }
  return buttons;  
}

String SendHTML(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +=" <head> \n";
  ptr +="   <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> \n";
  ptr +="   <style> \n";
  ptr +="     html {font-family: Arial; display: inline-block; text-align: center;} \n";
  ptr +="     h2 {font-size: 3.0rem;} \n";
  ptr +="     p {font-size: 3.0rem;} \n";
  ptr +="     body {max-width: 600px; margin:0px auto; padding-bottom: 25px;} \n";
  ptr +="     .switch {position: relative; display: inline-block; width: 120px; height: 68px}  \n";
  ptr +="     .switch input {display: none} \n";
  ptr +="     .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px} \n";
  ptr +="     .slider:before {position: absolute; content: \"\"; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px} \n";
  ptr +="     input:checked+.slider {background-color: #2196F3} \n";
  ptr +="     input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)} \n";
  ptr +="   </style> \n";
  ptr +=" </head> \n";
  ptr +=" <body> \n";
  ptr +="   <h2>ESP Web Server</h2> \n";
  ptr += GenButton();
  ptr +=" <script>function toggleCheckbox(element) { \n";
  ptr +="   var xhr = new XMLHttpRequest(); \n";
  ptr +="   if(element.checked){ xhr.open(\"GET\", \"/update?relay=\"+element.id+\"&state=1\", true); } \n";
  ptr +="   else { xhr.open(\"GET\", \"/update?relay=\"+element.id+\"&state=0\", true); } \n";
  ptr +="   xhr.send(); \n";
  ptr +=" }</script> \n";
  ptr +=" </body> \n";
  ptr +=" </html> \n";  
  return ptr;
}
