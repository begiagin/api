#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SD.h>
#include <Arduino_JSON.h>
#include <FS.h>
#include "dev_api.h"
#include "config_loader.h"

const char* ssid = "MobileAP";          // Replace with your Wi-Fi SSID
const char* password = "Aa1364123110";  // Replace with your Wi-Fi password

enum FILE_TYPE {
  HTML = 0,
  CSS, 
  JS,
  JSON_FILE,
  CONFIG,
  FONT
} ;
File currentUploadFile;
String DIR_PATH[] = {"/", "/css/", "/js/", "/json/", "/config/", "/css/fonts/"};

ESP8266WebServer server(80);


void setup() {
  Serial.begin(115200);

  // Initialize SD card
  if (!SD.begin(SS)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  // Read Configuration file 
  readJsonString(SD, DIR_PATH[FILE_TYPE::CONFIG]);

  // Initialize and Load all Required files to load WEB Dashboard  
  ManageRoutes("index.html", FILE_TYPE::HTML);
  ManageRoutes("css/bootstrap.rtl.css", FILE_TYPE::CSS);
  ManageRoutes("css/style.css", FILE_TYPE::CSS);
  ManageRoutes("css/fonts/Vazir.ttf", FILE_TYPE::CSS);
  ManageRoutes("js/bootstrap.min.js", FILE_TYPE::JS);
  ManageRoutes("js/JQuery.js", FILE_TYPE::JS);
  ManageRoutes("js/sensors.js", FILE_TYPE::JS);
  ManageRoutes("js/settings.js", FILE_TYPE::JS);

  // Handle API Routes 
  ManageAPI(WiFi, SD);
  
  server.begin();
  Serial.println("HTTP server started");
}


void ManageAPI(ESP8266WiFiClass mainWIFI, SDClass sd){

  server.on("/a2d", [](){
    server.send(200,"application/json", readA2D());
  });
  server.on("/upload", HTTP_POST, []() { 
     server.send(200, "text/plain", "Upload Successfuly Done !");
    
  }, handleFileUpload);  

    server.on("/con_info", [mainWIFI](){
    server.send(200,"application/json", readConnectionProps(mainWIFI));
  });
  
    server.on("/hw_info", [](){
    server.send(200,"application/json", readHW());
  });

      server.on("/sd_info", [sd](){
    server.send(200,"application/json", getSDCardSize(sd));
  });
}
void loop() {
  server.handleClient();
}
  

void handleFileUpload(){
  HTTPUpload& upload = server.upload();
  FILE_TYPE ft = (FILE_TYPE)(server.arg("type").toInt());
  String fileName = upload.filename;
  fileName = DIR_PATH[(int)ft] + fileName;
  
  
  switch(upload.status){
    case UPLOAD_FILE_START :
      if(SD.exists(fileName)){
        SD.remove(fileName);
      }
      currentUploadFile = SD.open(fileName, FILE_WRITE);
      //Serial.println(fileName + " Created !");
      break;
    case UPLOAD_FILE_WRITE :
      //Serial.println(fileName + " is Writing !");
      if(currentUploadFile){
         currentUploadFile.write(upload.buf, upload.currentSize);
         
      }  
      break;
    case UPLOAD_FILE_END : 
      //Serial.println(fileName + " Creation Finished !");
       if(currentUploadFile){
        currentUploadFile.close();
        
       }  
       break;

  }
}

void ManageRoutes(String fileName, FILE_TYPE type){
  //Serial.println("/"+fileName);
  Uri uri = "/" + fileName;
  switch(type){
    case FILE_TYPE::HTML : 
       server.on(uri,[ fileName ](){
            //Serial.println(fileName);
            File file = SD.open("/" + fileName);  // Replace with your HTML file name
            if (file) {
              server.streamFile(file, "text/html");
              file.close();
            } else {
              server.send(200, "text/plain", "Failed to open HTML file");
            }        
       });
       break;
    case FILE_TYPE::JS :   
       server.on("/" + fileName,[ fileName](){
            //Serial.println(fileName);
            File file = SD.open("/" + fileName);  // Replace with your HTML file name
            if (file) {
              server.streamFile(file, "text/javascript");
              file.close();
            } else {
              server.send(200, "text/plain", "Failed to open HTML file");
            }        
       });
       break;  
   case FILE_TYPE::CSS :   
       server.on("/" + fileName,[ fileName](){
            //Serial.println(fileName);
            File file = SD.open("/" + fileName);  // Replace with your HTML file name
            if (file) {
              server.streamFile(file, "text/stylesheet");
              file.close();
            } else {
              server.send(200, "text/plain", "Failed to open CSS file");
            }        
       });
       break;
   case FILE_TYPE::FONT :   
       server.on("/" + fileName,[ fileName](){
            //Serial.println(fileName);
            File file = SD.open("/" + fileName);  // Replace with your HTML file name
            if (file) {
              server.streamFile(file, "application/x-font-ttf");
              file.close();
            } else {
              server.send(200, "text/plain", "Failed to open font file");
            }        
       });

       //application/x-font-ttf        
    default : 
       break;        
  }
}


