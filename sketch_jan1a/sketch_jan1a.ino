#include <ESP8266WiFi.h>  
#include <ESPAsyncWebServer.h>  
#include <SD.h>  
#include <SPI.h>  

const char* ssid = "MogileAp";         // Replace with your network SSID  
const char* password = "Aa1364123110"; // Replace with your network password  

AsyncWebServer server(80);  
const int chipSelect = D2; // CS pin for SD card  
void setup() {
    Serial.begin(115200);  
  
  // Set up the built-in LED pin  
  pinMode(LED_BUILTIN, OUTPUT);  
  digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off initially  

  // Connect to Wi-Fi  
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED) {  
    delay(1000);  
    Serial.println("Connecting to WiFi...");  
  }  
  Serial.println("Connected to WiFi");  
  Serial.println(WiFi.localIP()); // Print the IP address  

  // Initialize the SD card  
  if (!SD.begin(chipSelect)) {  
    Serial.println("SD Card initialization failed!");  
    return; // Stop if SD card initialization fails  
  }  
  Serial.println("SD Card initialized successfully");  

  // Define routes  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){  
    File file = SD.open("/index.html");  
    if (file) {  
      request->send(file, "text/html");  
      file.close();  
    } else {  
      request->send(404, "text/plain", "File not found");  
    }  
  });  

  server.on("/led/on", HTTP_GET, [](AsyncWebServerRequest *request){  
    digitalWrite(LED_BUILTIN, LOW); // Turn LED on  
    request->send(200, "text/plain", "LED is ON");  
  });  

  server.on("/led/off", HTTP_GET, [](AsyncWebServerRequest *request){  
    digitalWrite(LED_BUILTIN, HIGH); // Turn LED off  
    request->send(200, "text/plain", "LED is OFF");  
  });  

  server.onNotFound([](AsyncWebServerRequest *request){  
    request->send(404, "text/plain", "404: Not Found");  
  });  

  // Start the server  
  server.begin();  

}

void loop() {
  // put your main code here, to run repeatedly:

}
