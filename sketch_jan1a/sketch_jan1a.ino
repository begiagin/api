#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SD.h>
#include <ArduinoJson.h>
#include "connection.h"
#include "config_loader.h"
#include "def.h"
#include "UserMGR.h"
#include "board_handels.h"

File currentUploadFile;
UserManager mgr;

void boardCheckerSetup(){

  Serial.println("\r\n");
  pinMode(PNP1, OUTPUT);
  pinMode(Relay, OUTPUT);
  pinMode(NPN1, OUTPUT);
  pinMode(NPN2, OUTPUT);

  pinMode(RCLK, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(SER, OUTPUT);

  pinMode(ProxyUp, INPUT_PULLUP);
  pinMode(ProxyDown, INPUT_PULLUP);

  digitalWrite(PNP1, HIGH);     // Turn it OFF by default.
  SPIx80(HIGH);       // Reset spi------------------------
  //  goOTA();


  server.on("/MotorSet", Motor_control);
  server.on("/adcread", sensor_data);
  server.on("/RemoteNo", RemoteNoFUN);
  server.on("/checkey", Test12v);
  server.on("/check", checkFUN);
  server.begin();  
}

void setup() {
  Serial.begin(74880);

  StaticJsonDocument<BUFFER_SIZE> RAM_CFG;

  // Initialize SD card
  if (!SD.begin(SS)) {
    Serial.println("SD card initialization failed!");
    return;
  }

  mgr.loadUsersFromFile(&SD);

  // Check JSON Config file is Exist

  RAM_CFG = readJsonString(SD, DIR_PATH[FILE_TYPE::CONFIG] + CONFIG_FILE_NAMES[CONF_SECTION::NETWORK]);
  if (!RAM_CFG.isNull()) {
    // Get Network Mode
    auto HOTSPOT_MODE = ((int)RAM_CFG["mode"]) == 1 ? true : false;
    auto DHCP_ENABLE = ((int)RAM_CFG["dhcp"]) == 1 ? true : false;

    if (HOTSPOT_MODE) {
      if (DHCP_ENABLE) {
        connectToNetowrk(WiFi, NETWORK_MODE::HOTSPOT_DHCP, RAM_CFG);
      } else {
        connectToNetowrk(WiFi, NETWORK_MODE::HOTSPOT, RAM_CFG);
      }
    } else {
      if (DHCP_ENABLE) {
        connectToNetowrk(WiFi, NETWORK_MODE::ACCESS_POINT_DHCP, RAM_CFG);
      } else {
        connectToNetowrk(WiFi, NETWORK_MODE::ACCESS_POINT, RAM_CFG);
      }
    }
  }

  // Handle Login endpoit
  handleLogin();

  // Initialize and Load all Required files to load WEB Dashboard
  loadHTMLFiles();

  // Board Checker Setup 
  boardCheckerSetup();

  // Check client ip address for checking Login Status
  isThisIpLoggedIn();

  // Handle API Routes
  ManageAPI(WiFi, SD, RAM_CFG);
  
  // Initial HTTP Server and Start it , Start listening on PORT 80, answer all incomming Requests 
  server.begin();
  Serial.println("HTTP server started");

}


void ManageAPI(ESP8266WiFiClass& mainWIFI, SDClass& sd,
               StaticJsonDocument<1024>& CFG) {

  ESP8266WebServer* webServer = &server;

  //Endpoint for Upload all Types of file
  server.on(
    "/upload", HTTP_POST, []() {
      server.send(200, "text/plain", "Upload Successfuly Done !");
    },
    handleFileUpload);

  // Endpoint for Read Connection Properties
  server.on("/con_info", [mainWIFI]() {
    //TODO remove function and replace With Obtained_Dev_Ip
    server.send(200, "application/json", "{ \n \"ip_addr\":\"" + OBTAINED_DEV_IP + "\"\n}");
  });

  server.on("/get-net-config", [CFG]() {
    String output;
    auto net_setting = readJsonString(SD, DIR_PATH[FILE_TYPE::CONFIG] + CONFIG_FILE_NAMES[CONF_SECTION::NETWORK]);

    serializeJson(net_setting, output);
    Serial.println("Get Config with : " + output);
    server.send(200, "application/json", output);
    delay(1000);
  });

  server.on(
    "/change-net_config", HTTP_POST, [webServer]() {
      auto postResult = postJSON(webServer, CONF_SECTION::NETWORK, SD);
      if (postResult == POST_JSON_RESULT::SUCCESS_RQ) {
      }
    });
}
void loop() {
  server.handleClient();

  if (!digitalRead(NPN1) && digitalRead(ProxyDown)) {
    Serial.println("DOWN OVER");
    isDOWN = false;
    checkDOWN = true;
  }
  if (!digitalRead(NPN2) && digitalRead(ProxyUp)) {
    Serial.println("UP OVER");
    isUP = false;
    checkUP = true;
  }
  digitalWrite(NPN1, !isDOWN);
  digitalWrite(NPN2, !isUP);
}


void handleLogin() {
  server.on(
    "/login", HTTP_POST, []() {
      if (server.hasArg("plain")) {
        StaticJsonDocument<BUFFER_SIZE> doc;
        DeserializationError error = deserializeJson(doc, server.arg("plain").c_str());
        if (!error) {
          String user = doc["usr"];
          String pass = doc["pass"];

          User* u = mgr.findUser(user);
          if (u) {
            auto encPass = mgr.encryptPassword(pass);
            if (encPass == u->getPassword()) {
              mgr.createSession(u->getUsername());
              u->isLogin = true;
              u->IPAddr = server.client().remoteIP().toString();
              server.sendHeader("SessionId", u->getSessionId());
              ManageRoutes("index.html", FILE_TYPE::HTML);
              server.sendHeader("Session-Expiretion", String(u->getSessionExpiration()));
              server.send(200, "text/plain", "User and Pass is correct");
            }
          } else {
            server.send(403, "text/plain", "Forbiden");
          }
        }
      }
    });
}

void handleFileUpload() {
  HTTPUpload& upload = server.upload();
  FILE_TYPE ft = (FILE_TYPE)(server.arg("type").toInt());
  String fileName = upload.filename;
  fileName = DIR_PATH[(int)ft] + fileName;


  switch (upload.status) {
    case UPLOAD_FILE_START:
      if (SD.exists(fileName)) {
        SD.remove(fileName);
      }
      currentUploadFile = SD.open(fileName, FILE_WRITE);
      break;
    case UPLOAD_FILE_WRITE:
      if (currentUploadFile) {
        currentUploadFile.write(upload.buf, upload.currentSize);
      }
      break;
    case UPLOAD_FILE_END:
      if (currentUploadFile) {
        currentUploadFile.close();
      }
      break;
  }
}

void redirectToLogin() {

  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "0");
  server.sendHeader("Location", "http://" + OBTAINED_DEV_IP + "/login.html", true);
  server.send(302, "text/plain", "Redirecting...");
  Serial.print("Redirecting to : ");
  Serial.println("http://" + OBTAINED_DEV_IP + "/login.html");
}

void isThisIpLoggedIn() {

  server.on("/", []() {
    User* u = mgr.findUserByIp(server.client().remoteIP().toString());
    if (u != nullptr) {
      // TODO Extrac Check
    } else {
      redirectToLogin();
      return;
    }
  });
}

void loadHTMLFiles() {
  ManageRoutes("login.html", FILE_TYPE::HTML);
  ManageRoutes("commands.html", FILE_TYPE::HTML);
  ManageRoutes("css/bootstrap.rtl.css", FILE_TYPE::CSS);
  ManageRoutes("css/style.css", FILE_TYPE::CSS);
  ManageRoutes("css/fonts/Vazir.ttf", FILE_TYPE::CSS);
  ManageRoutes("js/bootstrap.min.js", FILE_TYPE::JS);
  ManageRoutes("js/JQuery.js", FILE_TYPE::JS);
  ManageRoutes("js/sensors.js", FILE_TYPE::JS);
  ManageRoutes("js/settings.js", FILE_TYPE::JS);
  ManageRoutes("js/api.js", FILE_TYPE::JS);
  ManageRoutes("js/const-def.js", FILE_TYPE::JS);
  ManageRoutes("js/ui-evt.js", FILE_TYPE::JS);
}


void ManageRoutes(String fileName, FILE_TYPE type) {
  //Serial.println("/"+fileName);

  Uri uri = "/" + fileName;
  String fileTypeToRender = "";
  switch (type) {
    case FILE_TYPE::HTML:
      if (fileName != "login.html") {
      }
      fileTypeToRender = "text/html";
      break;
    case FILE_TYPE::JS:
      fileTypeToRender = "text/javascript";
      break;
    case FILE_TYPE::CSS:
      fileTypeToRender = "text/stylesheet";
      break;
    case FILE_TYPE::FONT:
      fileTypeToRender = "application/x-font-ttf";
      break;
    default:
      break;
  }

  server.on(uri, [fileName, fileTypeToRender]() {
    //Serial.println(fileName);
    File file = SD.open("/" + fileName);
    if (file) {
      server.streamFile(file, fileTypeToRender);
      file.close();
    } else {
      server.send(200, "application/json", POST_JSON_MESSAGES[POST_JSON_RESULT::DATA_NOT_FOUND]);
    }
  });
}
