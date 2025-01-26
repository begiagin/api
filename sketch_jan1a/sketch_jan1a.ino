#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SD.h>
#include <ArduinoJson.h>
#include "dev_api.h"
#include "connection.h"
#include "config_loader.h"
#include "def.h"
#include "UserMGR.h"

File currentUploadFile;

ESP8266WebServer server(80);
UserManager mgr;
          

void setup() {
  Serial.begin(115200);

  StaticJsonDocument<BUFFER_SIZE> RAM_CFG;

  // Initialize SD card
  if (!SD.begin(SS)) {
    Serial.println("SD card initialization failed!");
    return;
  }

  mgr.loadUsersFromFile(&SD);

  // Check JSON Config file is Exist

  RAM_CFG = readJsonString(SD, DIR_PATH[FILE_TYPE::CONFIG] + CONFIG_FILE_NAMES[CONF_SECTION::NETWORK]);
  if (RAM_CFG != null) {
    // Get Network Mode
    auto HOTSPOT_MODE = ((int)RAM_CFG["mode"]) == 1 ? true : false;
    auto DHCP_ENABLE = ((int)RAM_CFG["dhcp"]) == 1 ? true : false;
    Serial.println(HOTSPOT_MODE);
    Serial.println(DHCP_ENABLE);
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


  ManageRoutes("login.html", FILE_TYPE::HTML);
  ManageRoutes("css/bootstrap.rtl.css", FILE_TYPE::CSS);
  ManageRoutes("css/style.css", FILE_TYPE::CSS);
  ManageRoutes("css/fonts/Vazir.ttf", FILE_TYPE::CSS);
  ManageRoutes("js/bootstrap.min.js", FILE_TYPE::JS);
  ManageRoutes("js/JQuery.js", FILE_TYPE::JS);
  ManageRoutes("js/sensors.js", FILE_TYPE::JS);
  ManageRoutes("js/settings.js", FILE_TYPE::JS);
  ManageRoutes("js/data-validator.js", FILE_TYPE::JS);
  ManageRoutes("js/api.js", FILE_TYPE::JS);
  ManageRoutes("js/const-def.js", FILE_TYPE::JS);
  ManageRoutes("js/ui-evt.js", FILE_TYPE::JS);

  checkSessionId();


  // Handle API Routes
  ManageAPI(WiFi, SD, RAM_CFG);
  
  server.begin();
  Serial.println("HTTP server started");
}


void ManageAPI(ESP8266WiFiClass& mainWIFI, SDClass& sd,
               StaticJsonDocument<1024>& CFG) {

  server.on("/a2d", []() {
    server.send(200, "application/json", readA2D());
  });
  server.on(
    "/upload", HTTP_POST, []() {
      server.send(200, "text/plain", "Upload Successfuly Done !");
    },
    handleFileUpload);

  server.on("/con_info", [mainWIFI]() {
    //TODO remove function and replace With Obtained_Dev_Ip
    server.send(200, "application/json", readConnectionProps(mainWIFI));
  });

  server.on("/hw_info", []() {
    server.send(200, "application/json", readHW());
  });

  server.on("/sd_info", [sd]() {
    server.send(200, "application/json", getSDCardSize(sd));
  });

  server.on("/get-net-config", [CFG]() {
    String output;
    auto net_setting = readJsonString(SD, DIR_PATH[FILE_TYPE::CONFIG] + CONFIG_FILE_NAMES[CONF_SECTION::NETWORK]);

    serializeJson(net_setting, output);
    Serial.println("Get Config with : " + output);
    server.send(200, "application/json", output);
    delay(1000);
  });


  ESP8266WebServer* webServer = &server;

  server.on(
    "/change-net_config", HTTP_POST, [webServer]() {
      auto postResult = postJSON(webServer, CONF_SECTION::NETWORK, SD);
      if (postResult == POST_JSON_RESULT::SUCCESS_RQ) {
      }
    });
}
void loop() {
  server.handleClient();
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
      //Serial.println(fileName + " Created !");
      break;
    case UPLOAD_FILE_WRITE:
      //Serial.println(fileName + " is Writing !");
      if (currentUploadFile) {
        currentUploadFile.write(upload.buf, upload.currentSize);
      }
      break;
    case UPLOAD_FILE_END:
      //Serial.println(fileName + " Creation Finished !");
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

void checkSessionId() {
  
  server.on("/", []() {
    User* u = mgr.findUser(server.client().remoteIP().toString());
    if (u != nullptr) {
      // TODO Extrac Check
    } else {
      redirectToLogin();
      return;
    }
  });
}

void ManageRoutes(String fileName, FILE_TYPE type) {
  //Serial.println("/"+fileName);

  Uri uri = "/" + fileName;
  switch (type) {
    case FILE_TYPE::HTML:
      if (fileName != "login.html") {
      }

      server.on(uri, [fileName]() {
        //Serial.println(fileName);
        File file = SD.open("/" + fileName);  
        if (file) {
          server.streamFile(file, "text/html");
          file.close();
        } else {
          server.send(200, "text/plain", "Failed to open HTML file");
        }
      });
      break;
    case FILE_TYPE::JS:
      server.on("/" + fileName, [fileName]() {
        //Serial.println(fileName);
        File file = SD.open("/" + fileName);  
        if (file) {
          server.streamFile(file, "text/javascript");
          file.close();
        } else {
          server.send(200, "text/plain", "Failed to open HTML file");
        }
      });
      break;
    case FILE_TYPE::CSS:
      server.on("/" + fileName, [fileName]() {
        //Serial.println(fileName);
        File file = SD.open("/" + fileName);  
        if (file) {
          server.streamFile(file, "text/stylesheet");
          file.close();
        } else {
          server.send(200, "text/plain", "Failed to open CSS file");
        }
      });
      break;
    case FILE_TYPE::FONT:
      server.on("/" + fileName, [fileName]() {
        //Serial.println(fileName);
        File file = SD.open("/" + fileName);  
        if (file) {
          server.streamFile(file, "application/x-font-ttf");
          file.close();
        } else {
          server.send(200, "text/plain", "Failed to open font file");
        }
      });

      //application/x-font-ttf
    default:
      break;
  }
}
