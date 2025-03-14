
#include "IPAddress.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "def.h"



void connectToNetowrk(ESP8266WiFiClass& wifi, NETWORK_MODE mode,
                      StaticJsonDocument<BUFFER_SIZE>& cfg) {

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

  int connectTimeoutPeriod = 0;
  bool connectionStablished = true;


  switch (mode) {
    case NETWORK_MODE::HOTSPOT_DHCP:
      NetworkMode = "HOTSPT WITH DHCP ADDRESSING ";
      wifi.softAP(ssid_ap, password_ap);
      ip.fromString("192.168.8.8");
      gateway.fromString("192.168.8.1");
      subnetMask.fromString("255.255.255.0");
      wifi.softAPConfig(ip, gateway, subnetMask);
      Serial.println(wifi.softAPIP());
      OBTAINED_DEV_IP = wifi.softAPIP().toString();
      
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
      OBTAINED_DEV_IP = wifi.softAPIP().toString();
      break;
    case NETWORK_MODE::ACCESS_POINT_DHCP:

      if (strlen(CONFIG_SSID) > 0) {
        wifi.begin(CONFIG_SSID, CONFIG_WIFI_PASS);
      } else {
        wifi.begin(ssid_ap, password_ap);
      }
      NetworkMode = "ACCESS POINT WITH DHCP";
      Serial.println("Connecting to WiFi...");
      while (wifi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
        connectTimeoutPeriod++;
        if(connectTimeoutPeriod > 10){
          connectionStablished = false;
          break;
        }
      }
      OBTAINED_DEV_IP = wifi.localIP().toString();
      Serial.println("ESP IP Address : " + OBTAINED_DEV_IP);
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
      Serial.println("Connecting to WiFi...");
      NetworkMode = "ACCESS POINT WITH STATIC IP ADDRESS";
      while (wifi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
        connectTimeoutPeriod++;
        if(connectTimeoutPeriod > 10){
          connectionStablished = false;
          break;
        }        
      }
      OBTAINED_DEV_IP = wifi.localIP().toString();
      Serial.println("ESP IP Address : " + OBTAINED_DEV_IP);
      break;
  }
      if(!connectionStablished){

          NetworkMode = "HOTSPT WITH DHCP ADDRESSING ";
          wifi.softAP(ssid_ap, password_ap);
          ip.fromString("192.168.8.8");
          gateway.fromString("192.168.8.1");
          subnetMask.fromString("255.255.255.0");
          wifi.softAPConfig(ip, gateway, subnetMask);
          Serial.println(wifi.softAPIP());
          OBTAINED_DEV_IP = wifi.softAPIP().toString();  

      }

  Serial.println("Connected to WiFi . Network Mode : " + NetworkMode);
}