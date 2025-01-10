#include <SD.h>
#include <FS.h>

JSONVar readJsonString(SDClass SD, String configPath){
  File jsonFile = SD.open(configPath + "config.json");  
  JSONVar js = JSON.parse("\"{ }\"");
  if (!jsonFile) {  
    Serial.println("Error opening config.json");  
    return js;  
  }  

  // Read the file content into a string  
  String json;  
  while (jsonFile.available()) {  
    json += jsonFile.readString();  
  }  
  jsonFile.close();   
  js = JSON.parse(json);


  return js;

}

bool writeConfig(SDClass sd, JSONVar RAM_CONFIG, String configLocation){
  String jsonToWrite = JSON.stringify(RAM_CONFIG);
  File configFile = sd.open(configLocation + "config.json", FILE_WRITE);
  if(!configFile){
    Serial.println("Error in opening  for write config.json file");
    return false;
  }
  configFile.println(jsonToWrite);
  configFile.close();
  return true;
}

