#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include "time.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP_Mail_Client.h>

#define SMTP_HOST "smtp.gmail.com"
#define AUTHOR_EMAIL "esp32logs@gmail.com"
#define AUTHOR_PASSWORD "faje hymz hlmf nkue"
#define RECIPIENT_EMAIL "fulmali.tushar@gmail.com"
#define RECIPIENT_NAME "Tushar Fulmali"
#define SMTP_PORT 465  
SMTPSession smtp;
void smtpCallback(SMTP_Status status);

#define TIME_TO_SLEEP  15        /* Time ESP32 will go to sleep (in seconds) */
//#define Threshold 40 /* Greater the value, more the sensitivity */
#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
RTC_DATA_ATTR int bootCount = 0;

int OnBoardLed = 2;

#define NUM_RELAYS  4
int relayGPIOs[NUM_RELAYS] = {33, 13, 25, 32};

String msg ="";
IPAddress IP;

/* Put IP Address details */
IPAddress local_ip(192,168,1,100);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

/*Put your SSID & Password*/
// char* ssid = "_Naruto_";  // Enter SSID here
// char* password = "WakeUpNow@5";  //Enter Password here
const char* ssid = "Airtel_tush_9922";  // Enter SSID here
const char* password = "air33477";  //Enter Password here

bool retryWifiAfterAnHour = false;
int wifiLastRetry = 0;
String hostname = "ESP32 Irrigation";
const char* ssid2 = "ESP32";
const char* password2 = "12345678";

ESP_Mail_Session session;
SMTP_Message message;

WebServer server(80);

const char* ntpServer = "asia.pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;

unsigned long now = millis();
struct tm timeinfo;
int currentDate = 0;

struct Task{
  String title;
  int h;
  int m;
  int relay;
  bool isTurnOn;
  int goToSleepSec;
  bool isActive;
  bool isDone;
  //isOneTime, toggleAfterSec
};
int taskCapacity = 50;
int taskLength = 0;
struct Task taskList[50];

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

struct Button {
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  uint32_t lastTrigger;
  uint32_t timeSeconds;
  boolean startTimer;
};
Button button1 = {4, 0, 0, 15, false};
Button button2 = {16, 0, 0, 15, false};

bool processButton1 = false;
bool processButton2 = false;

void IRAM_ATTR button1Intp() {
  if (millis() - button1.lastTrigger > 500) {
    button1.lastTrigger = millis();
    button1.numberKeyPresses += 1;
    Serial.printf("Button 1 has been pressed %u times\n", button1.numberKeyPresses);
    processButton1 = true;
    //showMsg("Button 1 has been pressed ");
  }
}

void IRAM_ATTR button2Intp() {
  if (millis() - button2.lastTrigger > 500) {
    button2.lastTrigger = now;
    button2.numberKeyPresses += 1;
    button2.startTimer = true;
    Serial.printf("Button 2 has been pressed %u times\n", button2.numberKeyPresses);
    processButton2 = true;
  }
}

void setup(){
  pinMode(OnBoardLed, OUTPUT);

  Serial.begin(115200); delay(1000); //Take some time to open up the Serial Monitor

  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  print_wakeup_reason();
  // touchAttachInterrupt(T3, T3callback, Threshold);
  // esp_sleep_enable_touchpad_wakeup();
  // esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  // Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");

  pinMode(button1.PIN, INPUT_PULLUP);
  attachInterrupt(button1.PIN, button1Intp, FALLING);
  pinMode(button2.PIN, INPUT_PULLUP);
  attachInterrupt(button2.PIN, button2Intp, FALLING);

  for(int i=0; i<NUM_RELAYS; i++){
    pinMode(relayGPIOs[i], OUTPUT);
    digitalWrite(relayGPIOs[i], HIGH);
  }

  Serial.println("\n_initDisplay");
  initDisplay();

  // Temp change only
  Serial.println("\n_initWifi");
  initWifi();

  // Temp change only

// if(!getLocalTime(&timeinfo)){
//  Serial.println("\n_6");
//   initWifi();
//   initTime();
// }
  
//  WiFi.mode(WIFI_OFF);
//  btStop();
  // Temp change only

  Serial.println("\n_SMTP");
  MailClient.networkReconnect(true);
  smtp.debug(1);
  smtp.callback(smtpCallback);

  Session_Config config;
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = AUTHOR_EMAIL;
  config.login.password = AUTHOR_PASSWORD;
  config.login.user_domain = "";
  config.time.ntp_server = F("asia.pool.ntp.org,pool.ntp.org,time.nist.gov");
  config.time.gmt_offset = gmtOffset_sec;
  config.time.day_light_offset = daylightOffset_sec;

  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;

  message.sender.name = "ESP Mail";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "Wifi Connected";
  message.addRecipient(RECIPIENT_NAME, RECIPIENT_EMAIL);
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

  // Send plain email 
  String result = sendEmail("Wifi Connected", "Wifi Connected", false);

    // Send HTML email
    //char *body = "<b>Hello World!</b><br><p>This is an <i>HTML email</i> body";
    //String result = sendEmail("Test", body, true);

  Serial.println("\n_Server");
  server.on("/", handle_OnConnect);
  server.on("/update", handle_update);
  server.on("/deleteTask", handle_deleteTask);
  server.on("/insertTask", handle_insertTask);
  server.on("/connectWifi", handle_connectWifi);
  
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");
  showMsg("HTTP server started");

  //title:h:m:relay:isTurnOn:goToSleepSec:isActive:isDone
  //relay starts with 0
  Task newTask1 = {"Task 1", 9, 0, 0, true, 0, true, false};
  Task newTask2 = {"Task 2", 9, 5, 0, false, 0, true, false}; //82800s = 23H
  Task newTask3 = {"Task 3", 10, 0, 0, true, 0, true, false};
  Task newTask4 = {"Task 4", 10, 10, 0, false, 0, true, false}; //82800s = 23H
  insertTask(newTask1);
  insertTask(newTask2);
  insertTask(newTask3);
  insertTask(newTask4);
  listTask();
}

void lookForTask(){
  if ( currentDate != timeinfo.tm_yday){
    currentDate = timeinfo.tm_yday;
    resetTasks();
  }
  for (int i = 0; i < taskLength; i++)  
  {
    if (taskList[i].h == timeinfo.tm_hour && taskList[i].m == timeinfo.tm_min && taskList[i].isActive == true && taskList[i].isDone == false){
      performTask(i);
    }
  }  
}

void checkWifi(){
  if (WiFi.status() != WL_CONNECTED && timeinfo.tm_hour != wifiLastRetry){
    wifiLastRetry = timeinfo.tm_hour;
    initWifi();
  }
}

String sendEmail(char *subject, char *body, boolean htmlFormat) {
  message.subject = subject;

  if (htmlFormat){
    message.html.content = body;
    message.html.charSet = "utf-8";
  }
  else{
    message.text.content = body;
    message.text.charSet = "us-ascii";
  }
  

  /* Connect to the server */
  //if (!smtp.connect(&config)){  
  if (!smtp.connect(&session)){
    ESP_MAIL_PRINTF("Connection error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
    Serial.println("\nSMTP Connection error.");
    return "";
  }

  if (!smtp.isLoggedIn()){
    Serial.println("\nNot yet logged in.");
  }
  else{
    if (smtp.isAuthenticated())
      Serial.println("\nSuccessfully logged in.");
    else
      Serial.println("\nConnected with no Auth.");
  }

  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error sending Email, " + smtp.errorReason());
    
  ESP_MAIL_PRINTF("Free Heap: %d\n", MailClient.getFreeHeap());
  return "";
}

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status)
{
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  //if (status.success())
  //{
  //  Serial.println("----------------");
  //  ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
  //  ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
  //  Serial.println("----------------\n");
  //  struct tm dt;
  //
  //  for (size_t i = 0; i < smtp.sendingResult.size(); i++)
  //  {
  //    /* Get the result item */
  //    SMTP_Result result = smtp.sendingResult.getItem(i);
  //    time_t ts = (time_t)result.timestamp;
  //    localtime_r(&ts, &dt);
  //
  //    ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
  //    ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
  //    ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
  //    ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients);
  //    ESP_MAIL_PRINTF("Subject: %s\n", result.subject);
  //  }
  //  Serial.println("----------------\n");
  //}
}

void resetTasks(){
  for (int i = 0; i < taskLength; i++)  
  {
    taskList[i].isDone = false;
  }  
}

void performTask(int taskIndex){
  //tm_wday  int days since Sunday 0-6
  Serial.print("Title: " + String(taskList[taskIndex].title) + " HH:MM ");
  Serial.print(taskList[taskIndex].h);
  Serial.print(":");
  Serial.print(taskList[taskIndex].m);
  Serial.println();
  if (taskList[taskIndex].isTurnOn == true){
    digitalWrite(relayGPIOs[taskList[taskIndex].relay], LOW);
  }else{
    digitalWrite(relayGPIOs[taskList[taskIndex].relay], HIGH);
  }
  taskList[taskIndex].isDone = true;
  if (taskList[taskIndex].goToSleepSec > 0){
    goToSleep(taskList[taskIndex].goToSleepSec);
  }

  // Send plain email 
  String result = sendEmail("Task Completed", "Task Completed", false);
}

void loop(){
  server.handleClient();

  // Current time
  now = millis();

  updateDisplay();
  
  checkWifi();
  lookForTask();  
  
  if (processButton1){
    processButton1 = false;
    showMsg("Button 1 pressed");
    if (digitalRead(relayGPIOs[0]) == LOW){
      digitalWrite(relayGPIOs[0], HIGH);
    }else{
      digitalWrite(relayGPIOs[0], LOW);
    }
  }
  if (processButton2){
    processButton2 = false;
    showMsg("Button 2 pressed");
    if (digitalRead(relayGPIOs[1]) == LOW){
      digitalWrite(relayGPIOs[1], HIGH);
      button2.startTimer = false;
    }else{
      digitalWrite(relayGPIOs[1], LOW);
    }
  }
  
  if(button2.startTimer && (now - button2.lastTrigger > (button2.timeSeconds*1000))) {
    Serial.println("Time Expired for button 2...");
    String str = "lastTrigger ";
    str += button2.lastTrigger;
    Serial.println(str);
    String str2 = "now ";
    str2 += now;
    Serial.println(str2);
    digitalWrite(relayGPIOs[1], HIGH);
    button2.startTimer = false;
  }

}


void initDisplay(){
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
}

void initWifi(){
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);

  //WiFi.mode(WIFI_STA);
  //WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  //WiFi.setHostname(hostname.c_str()); //define hostname
    
  if (WiFi.status() != WL_CONNECTED){
    WiFi.begin(ssid, password);
  }

  int maxWait = 15;
  while (WiFi.status() != WL_CONNECTED && maxWait-- > 0) {
    digitalWrite(OnBoardLed, HIGH);
    delay(1000);
    digitalWrite(OnBoardLed, LOW);
    delay(1000);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED){
    Serial.println(WiFi.status());
    Serial.println("WiFi connected.");
    Serial.print("Got IP: ");  Serial.println(WiFi.localIP()); 
    Serial.print("Got MAC: ");  Serial.println(WiFi.macAddress());
    Serial.println("To list connected devices: cmd > arp -a");

    // SMTP needs time info
    initTime();

    msg = "WiFi connected \n"; 
    msg += "IP: ";
    IP = WiFi.localIP();
    msg += IP.toString().c_str();
    msg += "\n" ;
    msg += "MAC: " ;
    msg += WiFi.macAddress();
    showMsg(msg);
  }
  else if (retryWifiAfterAnHour == true){
    Serial.println("");
    Serial.println("WiFi NOT connected. Will retry after an hour.");
    msg = "WiFi NOT connected. Will retry after an hour."; 
    showMsg(msg);
    startOwnAP();
  }
  else{
    Serial.println("");
    Serial.println("WiFi NOT connected. Going to sleep for an hour.");
    msg = "WiFi NOT connected. Going to sleep for an hour."; 
    showMsg(msg);
    goToSleep(20);
  }
  
}

void startOwnAP(){
  WiFi.disconnect(true);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid2, password2);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

}

void initTime(){
  int maxWait = 15;
  while (!getLocalTime(&timeinfo) && maxWait-- > 0) {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    
    digitalWrite(OnBoardLed, HIGH);
    delay(1000);
    digitalWrite(OnBoardLed, LOW);
    delay(1000);
    Serial.print(".");
  }

  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    goToSleep(20);
    return;
  }

  // Init and get the time
  updateDisplay();
}

void updateDisplay(){
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    initTime();
    return;
  }

  Serial.print("Got IP: ");  Serial.println(WiFi.localIP()); 
  Serial.println(&timeinfo, "%d %b %H:%M:%S %a");
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(&timeinfo, "%d %b %H:%M:%S %a");
  display.print("IP: "); display.println(IP.toString().c_str());
  display.display();
}

void showMsg(String msg){
  Serial.println(msg);
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(msg);
  display.display();
  delay(2000);
}

void handle_OnConnect() {
  Serial.println("handle_OnConnect");
  server.send(200, "text/html", SendHTML()); 
}

void handle_update() {
  Serial.println("handle_update");
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
void handle_connectWifi(){
  Serial.println("handle_connectWifi");
  String s;
  String p;
  //.toCharArray(str_array, str.length());
  
  if (server.hasArg("ssid") & server.hasArg("password") ) {
    s = String(server.arg("ssid"));
    ssid = &s[0];
    p = String(server.arg("password"));
    password = &p[0];
    initWifi();
  }
  else if (server.args() == 0 ){
    initWifi();
  }
  else {
    Serial.println("Invalid message sent");
  }

  server.send(200, "text/html", SendHTML()); 
}
void handle_deleteTask() {
  Serial.println("handle_deleteTask");
  int h;
  int m;
  int r;
  if (server.hasArg("h") & server.hasArg("m") & server.hasArg("r")) {
    h = server.arg("h").toInt() ;
    m = server.arg("m").toInt() ;
    r = server.arg("r").toInt() ;
    deleteElement(h , m , r );
  }
  else {
    Serial.println("Invalid message sent");
  }
      
  server.send(200, "text/html", SendHTML()); 
}
void handle_insertTask() {
  Serial.println("handle_insertTask");
  String t;
  int h;
  int m;
  int r;
  bool isTurnOn;
  int goToSleepSec;
  
  if (server.hasArg("t") & server.hasArg("h") & server.hasArg("m") & server.hasArg("r") & server.hasArg("isTurnOn") & server.hasArg("goToSleepSec")) {
    t = server.arg("t") ;
    h = server.arg("h").toInt() ;
    m = server.arg("m").toInt() ;
    r = server.arg("r").toInt() ;
    isTurnOn = server.arg("isTurnOn") ;
    goToSleepSec = server.arg("goToSleepSec").toInt() ;
    
    Task newTask1 = {server.arg("t"), h, m, r, isTurnOn, goToSleepSec, true, false}; 
    insertTask(newTask1);
  }
  else {
    Serial.println("Invalid message sent");
  }
      
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
    return "";
  }
  else {
    return "checked";
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
  ///const char index_html[] PROGMEM = R"rawliteral(
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +=" <head> \n";
  ptr +="   <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> \n";
  ptr +="   <style> \n";
  ptr +="     html {font-family: Arial; display: inline-block; text-align: center;} \n";
  ptr +="     h2 {font-size: 3.0rem;} \n";
  ptr +="     p {font-size: 3.0rem;} \n";
  ptr +="     body {margin:0px auto; padding-bottom: 25px;} \n";
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
  ptr +="   <input type='button' onClick='connectWifi()' value='Connect Wifi' /> \n";
  ptr +="   <br/> \n";
  ptr += GenButton();

  ptr +=" <br/> \n";
  //title:h:m:relay:isTurnOn:goToSleepSec:isActive:isDone    
  //ptr +="<h4>title:h:m:relay:isTurnOn:goToSleepSec:isActive:isDone</h4> \n";
  ptr +="   <table align='center' >\n";
  ptr +="     <thead>\n";
  ptr +="         <tr> <th>title</th> <th>h</th> <th>m</th> <th>relay</th> <th>isTurnOn</th> <th>goToSleepSec</th> <th>isActive</th> <th>isDone</th> <th>Action</th> </tr>\n";
  ptr +="         <tr> <th><input type='text' id='txtTasktitle'/> </th> <th><input type='text' id='txtTaskh'/></th> <th><input type='text' id='txtTaskm'/></th> <th><input type='text' id='txtTaskrelay'/></th> <th><input type='text' id='txtTaskisTurnOn'/></th> <th><input type='text' id='txtTaskgoToSleepSec'/></th> <th></th> <th></th> <th><input type='button' onClick='insertNewTask()' value='Create' /></th> </tr>\n";
  ptr +="     </thead>\n";
  ptr +="     <tbody>\n";
  ptr += GenTaskList();
  ptr +="     </tbody>\n";
  ptr +="   </table>\n";

  ptr +=" <script>function connectWifi() { \n";
  ptr +="  var ssid = prompt('Please enter your SSID'); \n";
  ptr +="  var password = prompt('Please enter your Password'); \n";
  ptr +="  var xhr = new XMLHttpRequest(); \n";
  ptr +="  xhr.open(\"GET\", \"/connectWifi?ssid=\"+ssid+\"&password=password\", true); \n";
  ptr +="  xhr.onreadystatechange = function() { \n";
  ptr +="        if (this.readyState == 4 && this.status == 200) { location.reload(); } \n";
  ptr +="    }; \n";
  ptr +="   xhr.send(); \n";
  ptr +=" }</script> \n";
  ptr +=" <script>function toggleCheckbox(element) { \n";
  ptr +="   var xhr = new XMLHttpRequest(); \n";
  ptr +="   if(element.checked){ xhr.open(\"GET\", \"/update?relay=\"+element.id+\"&state=0\", true); } \n";
  ptr +="   else { xhr.open(\"GET\", \"/update?relay=\"+element.id+\"&state=1\", true); } \n";
  ptr +="  xhr.onreadystatechange = function() { \n";
  ptr +="        if (this.readyState == 4 && this.status == 200) { location.reload(); } \n";
  ptr +="    }; \n";
  ptr +="   xhr.send(); \n";
  ptr +=" }</script> \n";
  ptr +=" <script>function deleteTask(h,m,r) { \n";
  ptr +="   var xhr = new XMLHttpRequest(); \n";
  ptr +="  xhr.onreadystatechange = function() { \n";
  ptr +="        if (this.readyState == 4 && this.status == 200) { location.reload(); } \n";
  ptr +="    }; \n";
  ptr +="   xhr.open(\"GET\", \"/deleteTask?h=\"+h+\"&m=\"+m+\"&r=\"+r , true); \n";
  ptr +="   xhr.send(); \n";
  ptr +=" }</script> \n";
  ptr +="<script> \n";
  ptr +=" function createTask(t,h,m,r,isTurnOn,goToSleepSec) { \n";
  ptr +="   var xhr = new XMLHttpRequest(); \n";
ptr +="  xhr.onreadystatechange = function() { \n";
ptr +="        if (this.readyState == 4 && this.status == 200) { location.reload(); } \n";
ptr +="    }; \n";
  ptr +="   xhr.open(\"GET\", \"/insertTask?t=\"+t+\"&h=\"+h+\"&m=\"+m+\"&r=\"+r+\"&isTurnOn=\"+isTurnOn+\"&goToSleepSec=\"+goToSleepSec , true); \n";
  ptr +="   xhr.send(); \n";
  ptr +=" } \n";
  ptr +="function insertNewTask(){ \n";
  ptr +="  let t=document.getElementById('txtTasktitle').value; \n";
  ptr +="  let h=document.getElementById('txtTaskh').value; \n";
  ptr +="  let m=document.getElementById('txtTaskm').value; \n";
  ptr +="  let r=document.getElementById('txtTaskrelay').value; \n";
  ptr +="  let isTurnOn=document.getElementById('txtTaskisTurnOn').value; \n";
  ptr +="  let goToSleepSec=document.getElementById('txtTaskgoToSleepSec').value; \n";
  ptr +="  createTask(t,h,m,r,isTurnOn,goToSleepSec); \n";
  ptr +="} \n";
  ptr +=" </script> \n";
  ptr +=" </body> \n";
  ptr +=" </html> \n";  
  return ptr;
}

int insertTask(struct Task newTask) 
{ 
  int taskExist = findTask(newTask.h, newTask.m, newTask.relay);
  if ( taskExist > -1 ){
    return taskExist;
  }
  if (taskLength >= taskCapacity) 
    return -1;   
  taskList[taskLength] = newTask; 
  taskLength = taskLength + 1;
  return (taskLength); 
}
int findTask(int hours, int minuites, int relay) 
{ 
    int i; 
    for (i = 0; i < taskLength; i++) 
        if (taskList[i].h == hours && taskList[i].m == minuites && taskList[i].relay == relay) 
            return i;   
    return -1; 
}
int deleteElement(int hours, int minuites, int relay) 
{  
    // Find position of element to be deleted  
    Serial.println("deleteElement");
    int pos = findTask(hours, minuites, relay);
  
    if (pos == - 1)  
    {  
        Serial.println("Element not found");
        return -1;  
    }  
  
  deleteElementAt(pos);
  return taskLength;  
}

int deleteElementAt(int pos) 
{  
    int i;  
    for (i = pos; i < taskLength - 1; i++)  
        taskList[i] = taskList[i + 1];  
    taskLength = taskLength - 1;
    return taskLength;  
}
void listTask(){
    for (int i = 0; i < taskLength; i++)  
    {
      Serial.print("Title: ");
      Serial.print(taskList[i].title);
      Serial.print(" HH:MM ");
      Serial.print(taskList[i].h);
      Serial.print(":");
      Serial.print(taskList[i].m);
      Serial.println();
    }  
}

String GenTaskList(){
  String strTasks ="";
  for (int i = 0; i < taskLength; i++)  
  {
    //title:h:m:relay:isTurnOn:goToSleepSec:isActive:isDone    
    //strTasks+= "<h4>" + String(taskList[i].title) + ": " + String(taskList[i].h) + ": " + String(taskList[i].m) + ": " + String(taskList[i].relay) + ": " + String(taskList[i].isTurnOn) + ": " + String(taskList[i].goToSleepSec) + ": " + String(taskList[i].isActive) + ": " + String(taskList[i].isDone) + "</h4>";
    strTasks+= "<tr> <td>"+String(taskList[i].title)+"</td> <td>"+String(taskList[i].h)+"</td> <td>"+String(taskList[i].m)+"</td> <td>"+String(taskList[i].relay)+"</td> <td>"+String(taskList[i].isTurnOn)+"</td> <td>"+String(taskList[i].goToSleepSec)+"</td> <td>"+String(taskList[i].isActive)+"</td> <td>"+String(taskList[i].isDone)+"</td> <td> <input type='button' onClick='deleteTask("+String(taskList[i].h)+","+String(taskList[i].m)+","+String(taskList[i].relay)+")' value='Delete' /> </td> </tr>";
  }  
  return strTasks;  
}

void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : 
      Serial.println("Wakeup caused by touchpad"); 
      print_wakeup_touchpad();
      break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

void print_wakeup_touchpad(){
  touch_pad_t touchPin = esp_sleep_get_touchpad_wakeup_status();

  switch(touchPin)
  {
    case 0  : Serial.println("Touch 0 detected on GPIO 4"); break;
    case 1  : Serial.println("Touch 1 detected on GPIO 0"); break;
    case 2  : Serial.println("Touch 2 detected on GPIO 2"); break; //connected to internal led don't use
    case 3  : Serial.println("Touch 3 detected on GPIO 15"); break;
    case 4  : Serial.println("Touch 4 detected on GPIO 13"); break;
    case 5  : Serial.println("Touch 5 detected on GPIO 12"); break;
    case 6  : Serial.println("Touch 6 detected on GPIO 14"); break;
    case 7  : Serial.println("Touch 7 detected on GPIO 27"); break;
    case 8  : Serial.println("Touch 8 detected on GPIO 33"); break;
    case 9  : Serial.println("Touch 9 detected on GPIO 32"); break;
    default : Serial.println("Wakeup not by touchpad"); break;
  }
}

void T3callback(){
  //placeholder callback function
}

void goToSleep(int timeToSleep){
  Serial.println("Going to sleep now for " + String(timeToSleep) + " Seconds");
  msg = "Going to sleep now for " + String(timeToSleep) + " Seconds"; 
  showMsg(msg);
  Serial.flush(); 
  esp_sleep_enable_timer_wakeup(timeToSleep * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}
