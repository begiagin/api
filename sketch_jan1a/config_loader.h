#include <SD.h>
#include <ArduinoJson.h>
#include "def.h"

void SPIx80(bool endRCLK) {

  uint8_t i = sizeof(SPI_Array);
  uint8_t serbt, b;
  while (i > 0) {
    serbt = SPI_Array[i - 1];

    //serbt=0xaa; //***************

    b = 0;
    while (b < 8) {
      digitalWrite(SER, (serbt & 0b10000000));
      serbt *= 2;
      digitalWrite(SRCLK, HIGH);
      b++;
      digitalWrite(SRCLK, LOW);
    }
    i--;
  }
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);
  digitalWrite(RCLK, endRCLK);
}

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

POST_JSON_RESULT postJSON(ESP8266WebServer* server, CONF_SECTION section, SDClass& Sd) {
  String arg = "plain";
  if (server->hasArg(arg) == false) {  // Check if there is data in the request
    server->send(200, "application/json", POST_JSON_MESSAGES[(int)POST_JSON_RESULT::BAD_STRUCTURE]);
    return POST_JSON_RESULT::BAD_STRUCTURE;
  }

  // Read JSON returned by Calling API from WEB
  StaticJsonDocument<BUFFER_SIZE> json;
  DeserializationError error = deserializeJson(json, server->arg(arg));
  if (error) {
    Serial.println("JSON Parse Error");
  }
  switch (section) {
    case CONF_SECTION::NETWORK:
      writeConfig(Sd, json, DIR_PATH[FILE_TYPE::CONFIG] + CONFIG_FILE_NAMES[CONF_SECTION::NETWORK]);
      break;
    case CONF_SECTION::PROG:
      break;
    case CONF_SECTION::SD_CARD:
      break;
  }
  server->send(200, "application/json", POST_JSON_MESSAGES[(int)POST_JSON_RESULT::SUCCESS_RQ]);

  return POST_JSON_RESULT::SUCCESS_RQ;
}
