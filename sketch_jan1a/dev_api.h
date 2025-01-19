#include "WString.h"
#include "def.h"
#include <Arduino_JSON.h>
#include <ArduinoJson.h>
#include <SD.h>

JSONVar RAM_CONF;




String readA2D() {
  JSONVar A2D;
  A2D["value"] = (unsigned long)analogRead(A0);
  String jsonString = JSON.stringify(A2D);

  return jsonString;
}

String readConnectionProps(ESP8266WiFiClass wifi) {
  JSONVar WIFI;
  WIFI["ip_addr"] = wifi.localIP().toString();
  String jsonString = JSON.stringify(WIFI);
  return jsonString;
}

String getSDCardSize(SDClass sd) {
  uint32_t cardSize = sd.size();  // Gets the size of the card in bytes


  JSONVar sdinfo;
  sdinfo["card_size"] = cardSize;

  String jsonString = JSON.stringify(sdinfo);
  return jsonString;
}

String readHW() {
  JSONVar HW;
  HW["hw_ver"] = ESP.getFullVersion();
  String jsonString = JSON.stringify(HW);

  return jsonString;
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
  if(error){
    Serial.println("JSON Parse Error");  
  }
  switch (section) {
    case CONF_SECTION::NETWORK :
      writeConfig(Sd, json, DIR_PATH[FILE_TYPE::CONFIG]+
                            CONFIG_FILE_NAMES[CONF_SECTION::NETWORK]);
      break;
    case CONF_SECTION::PROG :
      break;   
    case CONF_SECTION::SD_CARD :  
      break; 
  }
  server->send(200, "application/json", POST_JSON_MESSAGES[(int)POST_JSON_RESULT::SUCCESS]);

  return POST_JSON_RESULT::SUCCESS;
}
