#include <SD.h>
#include <FS.h>

JSONVar readJsonString(SDClass& SD, String configPath, JSONVar& net_struct){
  File jsonFile = SD.open(configPath);  
  JSONVar js = JSON.parse("{}");
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
  Serial.println(json);
  js = JSON.parse(json);


  return js;

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

