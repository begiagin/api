#include <ESP8266WiFi.h>

enum NETWORK_MODE {
  HOTSPOT = 0,
  HOTSPOT_DHCP,
  ACCESS_POINT,
  ACCESS_POINT_DHCP
};

const char* ssid_sta = "MobileAP";            // Your WiFi network SSID   
const char* password_sta = "Aa1364123110";    // Your WiFi network password  

const char* ssid_ap = "ESP8266_AP";                 // Access Point SSID  
const char* password_ap = "12345678";               // Access Point password 

void connectToNetowrk(NETWORK_MODE mode, ESP8266WiFiClass& wifi){

}