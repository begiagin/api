#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SD.h>

const char* ssid = "MobileAP";          // Replace with your Wi-Fi SSID
const char* password = "Aa1364123110";  // Replace with your Wi-Fi password

enum FILE_TYPE {
  HTML = 1,
  JS,
  CSS, 
  FONT
} ;

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

  // Initialize and Load all Required files to load WEB Dashboard
  
  ManageRoutes("index.html", FILE_TYPE::HTML);
  ManageRoutes("css/bootstrap.rtl.css", FILE_TYPE::CSS);
  ManageRoutes("css/style.css", FILE_TYPE::CSS);
  ManageRoutes("css/fonts/Vazir.ttf", FILE_TYPE::CSS);
  ManageRoutes("js/bootstrap.min.js", FILE_TYPE::JS);
  ManageRoutes("js/JQuery.js", FILE_TYPE::JS);
  ManageRoutes("js/sensors.js", FILE_TYPE::JS);
  ManageRoutes("js/settings.js", FILE_TYPE::JS);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void ManageRoutes(String fileName, FILE_TYPE type){
  //Serial.println("/"+fileName);
  Uri uri = "/" + fileName;
  switch(type){
    case FILE_TYPE::HTML : 
       server.on(uri,[ fileName ](){
            Serial.println(fileName);
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
            Serial.println(fileName);
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
            Serial.println(fileName);
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
            Serial.println(fileName);
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


