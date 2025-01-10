#include <SD.h>
#include <FS.h>



String readJsonString(SDClass SD, String configPath){
  File jsonFile = SD.open(configPath + "config.json");  
  if (!jsonFile) {  
    Serial.println("Error opening config.json");  
    return "";  
  }  

  // Read the file content into a string  
  String json;  
  while (jsonFile.available()) {  
    json += jsonFile.readString();  
  }  
  jsonFile.close();   
  
  JSONVar js = JSON.parse(json);

  Serial.println(json);

  return json;

}

String getConfigByKey(){
  
  // Read configuration Stored in SD card
  //String mConfig = readJsonConfiguration(SD, DIR_PATH[FILE_TYPE::CONFIG]);

  return "";
}