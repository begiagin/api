#include "IPAddress.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

enum NETWORK_MODE {
  HOTSPOT = 0,
  HOTSPOT_DHCP,
  ACCESS_POINT,
  ACCESS_POINT_DHCP
};

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

const char* ssid_sta = "MobileAP";          // Your WiFi network SSID
const char* password_sta = "Aa1364123110";  // Your WiFi network password

const char* ssid_ap = "ESP8266_AP";    // Access Point SSID
const char* password_ap = "12345678";  // Access Point password

void connectToNetowrk(ESP8266WiFiClass& wifi, NETWORK_MODE mode, StaticJsonDocument<BUFFER_SIZE>& cfg) {

  // Connect to Wi-Fi

  const char* CONFIG_SSID = (const char*)cfg["wn"];
  const char* CONFIG_WIFI_PASS = (const char*)cfg["wp"];

  IPAddress ip;
  IPAddress subnetMask;
  IPAddress gateway;
  IPAddress primaryDNS;
  IPAddress secondaryDNS;

  const char* IP = (const char*)cfg["ip"];
  const char* SUBNET_MASK = (const char*)cfg["sm"];
  const char* GATEWAY = (const char*)cfg["gw"];
  const char* PRIMARY_DNS = (const char*)cfg["pd"];
  const char* SECONDARY_DNS = (const char*)cfg["sd"];
  String NetworkMode = "";



  switch (mode) {
    case NETWORK_MODE::HOTSPOT_DHCP:
      NetworkMode = "HOTSPT WITH DHCP ADDRESSING ";    
      
      if (strlen(CONFIG_SSID) > 0) {
        wifi.softAP(CONFIG_SSID, CONFIG_WIFI_PASS);
      } else {
        wifi.softAP(ssid_ap, password_ap);
      }    
      break;
    case NETWORK_MODE::HOTSPOT:

      ip.fromString(IP);
      subnetMask.fromString(SUBNET_MASK);
      gateway.fromString(GATEWAY);
      primaryDNS.fromString(PRIMARY_DNS);
      secondaryDNS.fromString(SECONDARY_DNS);

      wifi.softAPConfig(ip, gateway, subnetMask);
      
      NetworkMode = "HOTSPOT WITH STATIC IP ADDRESSING ";    
      
      if (strlen(CONFIG_SSID) > 0) {
        wifi.softAP(CONFIG_SSID, CONFIG_WIFI_PASS);
      } else {
        wifi.softAP(ssid_ap, password_ap);
      }

      break;
    case NETWORK_MODE::ACCESS_POINT_DHCP:

      if (strlen(CONFIG_SSID) > 0) {
        wifi.begin(CONFIG_SSID, CONFIG_WIFI_PASS);
      } else {
        wifi.begin(ssid_ap, password_ap);
      }
      NetworkMode = "ACCESS POINT WITH DHCP";

      break;
    case NETWORK_MODE::ACCESS_POINT:


      ip.fromString(IP);
      subnetMask.fromString(SUBNET_MASK);
      gateway.fromString(GATEWAY);
      primaryDNS.fromString(PRIMARY_DNS);
      secondaryDNS.fromString(SECONDARY_DNS);

      wifi.config(ip, gateway, subnetMask, primaryDNS, secondaryDNS);
 
      if (strlen(CONFIG_SSID) > 0) {
        wifi.begin(CONFIG_SSID, CONFIG_WIFI_PASS);
      } else {
        wifi.begin(ssid_ap, password_ap);
      }

      NetworkMode = "ACCESS POINT WITH STATIC IP ADDRESS";
      break;
  }

  while (wifi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  //Serial.println(wifi.localIP());
}