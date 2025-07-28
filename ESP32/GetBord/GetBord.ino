void setup() {
  #if defined(CONFIG_IDF_TARGET_ESP32S2)
    Serial.println("CONFIG_IDF_TARGET_ESP32S2");
  #elif defined(CONFIG_IDF_TARGET_ESP32S3)
    Serial.println("CONFIG_IDF_TARGET_ESP32S3");
  #elif defined(CONFIG_IDF_TARGET_ESP32C3)
    Serial.println("CONFIG_IDF_TARGET_ESP32C3");
  #else
    Serial.println("Else");
  #endif
}

void loop() {
}
