
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
ESP8266WebServer server(80);
#endif

#ifndef PREFIX_SALT
#define PREFIX_SALT "HaLATA"
#endif

#ifndef POSTFIX_SALT
#define POSTFIX_SALT "NAMEOFGREATEGOD"
#endif


#ifndef POST_RESULT_H  
#define POST_RESULT_H  

enum POST_JSON_RESULT {
  SUCCESS_RQ = 0,
  NOT_OK,
  BAD_STRUCTURE,
  DATA_NOT_FOUND
};

#endif // POST_RESULT_H

#ifndef CONF_SECTION_H  
#define CONF_SECTION_H  
enum CONF_SECTION {
  NETWORK = 0,
  SD_CARD,
  MEMEORY,
  BEHAVIOR,
  PROG,
  USERS_JSON_FILE
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

enum FILE_TYPE {
  HTML = 0,
  CSS,
  JS,
  JSON_FILE,
  CONFIG,
  FONT, 
  USERS_JSON
};

String POST_JSON_MESSAGES[] = {
  "{ \"data\": \"SUCCESS\" }",
  "{ \"data\": \"FAIL\" }",
  "{ \"data\": \"BAD_ARG\" }",
  "{ \"data\": \"DATA_NOT_FOUND\" }"
};
String DIR_PATH[] = { "/", "/css/", "/js/", "/json/", "/config/", "/css/fonts/", "/data/" };

const char* ssid_sta = "MobileAP";          // Your WiFi network SSID
const char* password_sta = "Aa1364123110";  // Your WiFi network password

const char* ssid_ap = "ESP8266_AP";    // Access Point SSID
const char* password_ap = "12345678";  // Access Point password



String OBTAINED_DEV_IP;
String CONFIG_FILE_NAMES[] = {
  "network_config.json", 
  "sd_card.json", 
  "memory.json",
  "behavior.json",
  "prog.json",
  "users.json"
};
//String NETOWRK_JSON_KEYS = {"mode","dhcp","ip","gw","sm","pd","sd","wn","wp"};


// Board Checker Variables 

#endif // DEFINE_MAIN_VARIABLES_H

bool writeConfig(SDClass sd, StaticJsonDocument<BUFFER_SIZE> RAM_CONFIG, String configLocation);
void set_comm(char Rmt[6]);

#ifndef BOARD_CHECKER_H
#define BOARD_CHECKER_H

#define PNP1          0
#define Relay         2
#define NPN1          4
#define NPN2          15
#define ProxyDown     5
#define ProxyUp       16
#define RCLK          12
#define SRCLK         14
#define SER           13


/* BEGIN OF BOARD CHECKER VARIABLES */
bool isOTA;
bool isUP, isDOWN;
bool checkUP, checkDOWN;

uint8_t SPI_Array[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
uint8_t Key1_4, Remote1_20, ProgGroup1_10;
/* END OF BOARD CHECKER VARIABLES */

#endif






