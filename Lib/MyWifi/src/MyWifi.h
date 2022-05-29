#ifndef MyWifi_H
#define MyWifi_H

  #include <Arduino.h>
  #include <WiFi.h>
  #include <ESP_Mail_Client.h>

  //#include <MySleep.h>
  #include <MyDisplay.h>
  extern MyDisplay myDisplay;
  //extern struct tm timeinfo;

  /// Email configuration
  #define SMTP_HOST "smtp.gmail.com"
  #define AUTHOR_EMAIL "esp32logs@gmail.com"
  #define AUTHOR_PASSWORD "Aspirine@123"
  #define RECIPIENT_EMAIL "esp32logs@gmail.com"
  #define RECIPIENT_NAME "Tushar Fulmali"
  #define SMTP_PORT 465  

  void smtpCallback(SMTP_Status status);

class MyWifi
{
  private:
    /// Wifi configuration
    bool retryWifiAfterAnHour = true; //Will continue without internet
    char* Wifi_AP_Name = "ESP32 HomeAutomation";
    char* Wifi_AP_Pass = "12345678";
    char* Wifi_Name = "_Naruto_";
    char* Wifi_Pass = "WakeUpNow@5";
    //const char* Wifi_Name = "Manisha";
    //const char* Wifi_Pass = "Manisha12345";  //Enter Password here

    SMTPSession smtp;
    ESP_Mail_Session session;
    SMTP_Message message;

    String wifiMsg = "";
    int _ledPin = -1;
    void updateEmailConfiguration();
    void (*ptr)(SMTP_Status);

  public:
    MyWifi(int ledPin); // -1 identifies no led required
    IPAddress IP;
    uint32_t wifiLastRetry = 0;
    String sendEmail(char *subject, char *body, boolean htmlFormat);
    void initWifi(boolean needToWait = false);
    void startOwnAP();
    //void checkWifi();
};

#endif
