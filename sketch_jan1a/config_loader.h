#include <SD.h>
#include <FS.h>
#include <ArduinoJson.h>

#define BUFFER_SIZE 1024          

JSONVar readJsonString(SDClass& SD, String configPath){
  File jsonFile = SD.open(configPath);  
  JSONVar jsonObject ;
  if (!jsonFile) {  
    Serial.println("Error opening config.json");  
    
    return jsonObject;  
  }  

  // Read the file content into a string  
  String jsonString;  
  while (jsonFile.available()) {  
    jsonString += (char)jsonFile.read();  
  }  
  //json = "{  \"ip\": \"192.168.0.59\", \"sm\": \"255.255.255.0\",  \"gw\": \"192.168.0.1\",  \"pd\": \"8.8.8.8\",  \"sd\": \"4.2.2.4\",  \"wn\": \"DELTA\",  \"wp\": \"Aa@1364123110\",  \"mode\": 0,  \"dhcp\": 0}";
  jsonFile.close();   
  Serial.println("");
  Serial.println(jsonString);
     // Create a StaticJsonDocument  
    StaticJsonDocument<BUFFER_SIZE> jsonDoc;  

    // Parse the JSON string  
    DeserializationError error = deserializeJson(jsonDoc, jsonString);  

  //jsonObject = JSON.parse(jsonString.c_str());
  if(!error){
    Serial.print("Mode = ");
    String IpAddr = jsonDoc["ip"];
    Serial.println(IpAddr);
  }
  return jsonObject;

}

bool writeConfig(SDClass sd, JSONVar RAM_CONFIG, String configLocation){
  
  File configFile = sd.open(configLocation + "config.json", FILE_WRITE);
  if(!configFile){
    Serial.println("Error in opening  for write config.json file");
    return false;
  }
  configFile.println(JSON.stringify(RAM_CONFIG));
  configFile.close();
  return true;
}

