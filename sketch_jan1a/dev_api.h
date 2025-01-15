#include "WString.h"

#include <Arduino_JSON.h>
#include <SD.h>

JSONVar RAM_CONF;
enum POST_JSON_RESULT {
  SUCCESS = 0,
  NOT_OK,
  BAD_STRUCTURE
};
String POST_JSON_MESSAGES[] = {
  "\"{ \"data\": \"SUCCESS\" }\"",
  "\"{ \"data\": \"FAIL\" }\"",
  "\"{ \"data\": \"BAD_ARG\" }\""
  };


String readA2D(){
   JSONVar A2D ;
   A2D["value"] = (unsigned long) analogRead(A0);
   String jsonString = JSON.stringify(A2D);

   return jsonString;
}

String readConnectionProps(ESP8266WiFiClass wifi){
   JSONVar WIFI ;
   WIFI["ip_addr"] = wifi.localIP().toString();
   String jsonString = JSON.stringify(WIFI);

   return jsonString;
}
 
String getSDCardSize(SDClass sd) {  
  uint32_t cardSize = sd.size();// Gets the size of the card in bytes  


  JSONVar sdinfo;
  sdinfo["card_size"] = cardSize;
  
  String jsonString = JSON.stringify(sdinfo);
  return jsonString;
}  

String readHW(){
  JSONVar HW;
  HW["hw_ver"] =ESP.getFullVersion() ;
  String jsonString = JSON.stringify(HW);

  return jsonString;
}

POST_JSON_RESULT postJSON(ESP8266WebServer* server, const String arg){
  if (server->hasArg(arg) == false) { // Check if there is data in the request 
    server->send(200, "application/json", POST_JSON_MESSAGES[(int)POST_JSON_RESULT::BAD_STRUCTURE]); 
    return POST_JSON_RESULT::BAD_STRUCTURE; 
  }

  JSONVar json = JSON.parse(server->arg(arg));
  RAM_CONF["ip"] = json["ip"];
  
  return POST_JSON_RESULT::SUCCESS;
}
