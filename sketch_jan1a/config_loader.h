#include <SD.h>
#include <FS.h>
#include <ArduinoJson.h>

#define BUFFER_SIZE 1024

StaticJsonDocument<BUFFER_SIZE> readJsonString(SDClass& SD, String configPath) {
  File jsonFile = SD.open(configPath);

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
  Serial.println("-------------");
  Serial.println(jsonString);
  Serial.println("-------------");

  // Parse the JSON string
  DeserializationError error = deserializeJson(jsonDoc, jsonString);

  if (!error) {
    Serial.print("Mode = ");
    String IpAddr = jsonDoc["ip"];
    Serial.println(IpAddr);
  }
  return jsonDoc;
}

bool writeConfig(SDClass sd, JSONVar RAM_CONFIG, String configLocation) {

  File configFile = sd.open(configLocation + "config.json", FILE_WRITE);
  if (!configFile) {
    Serial.println("Error in opening  for write config.json file");
    return false;
  }
  configFile.println(JSON.stringify(RAM_CONFIG));
  configFile.close();
  return true;
}
