#include <SD.h>
#include <FS.h>
#include <ArduinoJson.h>
#include "def.h"

StaticJsonDocument<BUFFER_SIZE> readJsonString(SDClass& SD, String configPath) {
  File jsonFile = SD.open(configPath);
  Serial.println(configPath);
  StaticJsonDocument<BUFFER_SIZE> jsonDoc;
  if (!jsonFile) {
    Serial.println("Error opening config.json");

    return jsonDoc;
  }

  // Read the file content into a string
  String jsonString;
  while (jsonFile.available()) {
    jsonString += (char)jsonFile.read();
  }
  jsonFile.close();


  // Parse the JSON string
  DeserializationError error = deserializeJson(jsonDoc, jsonString);

  if (!error) {
    Serial.print("Mode = ");
    String IpAddr = jsonDoc["ip"];
    Serial.println(IpAddr);
  }
  
  return jsonDoc;
}
bool writeConfig(SDClass sd, StaticJsonDocument<BUFFER_SIZE> CONFIG_SEC, String configLocation) {

  if (sd.exists(configLocation)) {
    sd.remove(configLocation);
  }
  File configFile = sd.open(configLocation, FILE_WRITE);

  if (!configFile) {
    Serial.println("Error in opening  for write config.json file");
    return false;
  }
  String jsonStrinContent = "";
  serializeJson(CONFIG_SEC, jsonStrinContent);
  Serial.println("Set Config with : " + jsonStrinContent);
  configFile.println(jsonStrinContent);
  configFile.close();
  return true;
}
