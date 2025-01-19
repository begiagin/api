#include <Arduino_JSON.h>
#include <ArduinoJson.h>
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

#ifndef POST_RESULT_H  
#define POST_RESULT_H  

enum POST_JSON_RESULT {
  SUCCESS = 0,
  NOT_OK,
  BAD_STRUCTURE
};

#endif // POST_RESULT_H

#ifndef CONF_SECTION_H  
#define CONF_SECTION_H  
enum CONF_SECTION {
  NETWORK = 0,
  SD_CARD,
  MEMEORY,
  BEHAVIOR,
  PROG
};
#endif // CONF_SECTION_H  

#ifndef NETWORK_MODE_H
#define NETWORK_MODE_H

enum NETWORK_MODE {
  HOTSPOT = 0,
  HOTSPOT_DHCP,
  ACCESS_POINT,
  ACCESS_POINT_DHCP
};
#endif // NETWORK_MODE_H

#ifndef DEFINE_MAIN_VARIABLES_H
#define DEFINE_MAIN_VARIABLES_H

String POST_JSON_MESSAGES[] = {
  "{ \"data\": \"SUCCESS\" }",
  "{ \"data\": \"FAIL\" }",
  "{ \"data\": \"BAD_ARG\" }"
};
String DIR_PATH[] = { "/", "/css/", "/js/", "/json/", "/config/", "/css/fonts/" };

const char* ssid_sta = "MobileAP";          // Your WiFi network SSID
const char* password_sta = "Aa1364123110";  // Your WiFi network password

const char* ssid_ap = "ESP8266_AP";    // Access Point SSID
const char* password_ap = "12345678";  // Access Point password

enum FILE_TYPE {
  HTML = 0,
  CSS,
  JS,
  JSON_FILE,
  CONFIG,
  FONT
};
String CONFIG_FILE_NAMES[] = {
  "network_config.json", 
  "sd_card.json", 
  "memory.json",
  "behavior.json",
  "prog.json"
};
//String NETOWRK_JSON_KEYS = {"mode","dhcp","ip","gw","sm","pd","sd","wn","wp"};

#endif // DEFINE_MAIN_VARIABLES_H

bool writeConfig(SDClass sd, StaticJsonDocument<BUFFER_SIZE> RAM_CONFIG, String configLocation);








