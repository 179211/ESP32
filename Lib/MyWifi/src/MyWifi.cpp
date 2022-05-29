#include "MyWifi.h"

MyWifi::MyWifi(int ledPin = -1)
{
  _ledPin = ledPin;
  initWifi(true);
}

void MyWifi::initWifi(boolean needToWait = false){  
  if (_ledPin > 0){
      pinMode(_ledPin, OUTPUT);
  }

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(Wifi_Name);

  updateEmailConfiguration();

  WiFi.mode(WIFI_STA);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(Wifi_AP_Name); //define hostname
    
  if (WiFi.status() != WL_CONNECTED){
    WiFi.begin(Wifi_Name, Wifi_Pass);
  }

  if (needToWait){
    int maxWait = 15;
    while (WiFi.status() != WL_CONNECTED && maxWait-- > 0) {
      if (_ledPin > 0){
        digitalWrite(_ledPin, HIGH);
        delay(1000);
        digitalWrite(_ledPin, LOW);
        delay(1000);
      }
      else{
        delay(2000);
      }
      Serial.print(".");
    }
  }

  if (WiFi.status() == WL_CONNECTED){
    if (_ledPin > 0){
      digitalWrite(_ledPin, HIGH);
    }
    Serial.println(WiFi.status());
    Serial.println("WiFi connected.");
    Serial.print("Got IP: ");  Serial.println(WiFi.localIP()); 
    Serial.print("Got MAC: ");  Serial.println(WiFi.macAddress());
    Serial.println("To list connected devices: cmd > arp -a");
    wifiMsg = "WiFi connected \n"; 
    wifiMsg += "IP: ";
    IP = WiFi.localIP();
    wifiMsg += IP.toString().c_str();
    wifiMsg += "\n" ;
    wifiMsg += "MAC: " ;
    wifiMsg += WiFi.macAddress();
    myDisplay.showMsg(wifiMsg);
    
    String result = sendEmail("Wifi Connected", "Wifi Connected", false);
  }
  else if (retryWifiAfterAnHour == true){
    if (_ledPin > 0){
      digitalWrite(_ledPin, LOW);
    }
    Serial.println("");
    wifiMsg = "WiFi NOT connected. Will retry after an hour."; 
    Serial.println(wifiMsg);
    myDisplay.showMsg(wifiMsg);
    startOwnAP();
  }
  // else{
  //   Serial.println("");
  //   wifiMsg = "WiFi NOT connected. Going to sleep for 20 sec."; 
  //   Serial.println(wifiMsg);
  //   myDisplay.showMsg(wifiMsg);
  //   goToSleep(20);
  // }
  
}

void MyWifi::startOwnAP(){
  if (_ledPin > 0){
    digitalWrite(_ledPin, LOW);
  }
  IPAddress local_ip(192,168,1,1);
  IPAddress gateway(192,168,1,1);
  IPAddress subnet(255,255,255,0);

  WiFi.disconnect(true);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(Wifi_AP_Name, Wifi_AP_Pass);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  myDisplay.showMsg("AP IP address: ");
}

/// Send HTML email
//char *body = "<b>Hello World!</b><br><p>This is an <i>HTML email</i> body";
//String result = sendEmail("Test", body, true);
String MyWifi::sendEmail(char *subject, char *body, boolean htmlFormat = false) {
  if (WiFi.status() != WL_CONNECTED){
    return "Error sending Email: Wifi not available";
  }

  message.subject = subject;
  if (htmlFormat){
    message.html.content = body;
    message.html.charSet = "utf-8";
  }
  else{
    message.text.content = body;
    message.text.charSet = "us-ascii";
  }
  
  /* Connect to server with the session config */
  if (!smtp.connect(&session))
    return "Error sending Email: Session not created";

  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message)){
    Serial.println("Error sending Email, " + smtp.errorReason());
    return "Error sending Email: " + smtp.errorReason();
  }
    
  ESP_MAIL_PRINTF("Free Heap: %d\n", MailClient.getFreeHeap());
  return "";
}

void MyWifi::updateEmailConfiguration(){
    smtp.debug(1);
    ptr = &smtpCallback;
    smtp.callback(ptr);
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
}

//void (*ptr)(SMTP_Status) = &smtpCallback;
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

// void MyWifi::checkWifi(){
//   if (WiFi.status() != WL_CONNECTED && timeinfo.tm_hour != wifiLastRetry){
//     wifiLastRetry = timeinfo.tm_hour;
//     initWifi();
//   }
// }
