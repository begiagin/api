#include "WString.h"

#include <Arduino_JSON.h>
#include <SD.h>

JSONVar RAM_CONF;

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

void setNetworkSetting(){

}
