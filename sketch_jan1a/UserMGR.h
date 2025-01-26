#include "SD.h"
#include "Crypto.h"
#include <iostream>
#include <vector>
#include <FS.h>
#include <Seeed_mbedtls.h>
#include <ArduinoJson.h>

class User {
private:
  String Username;
  String Password;
  String SessionId;
  unsigned long SessionExpiration;  // زمان انقضا


public:
  String IPAddr;
  bool isLogin = false;

  User(){}
  User(String uName, String pWord)
    : Username(uName), Password(pWord) {}
  String getUsername() {
    return Username;
  }
  String toJson() const {
    DynamicJsonDocument doc(200);  // Create a JSON document
    doc["Username"] = Username;
    doc["Password"] = Password;
    doc["SessionId"] = SessionId;
    doc["SessionExpiration"] = SessionExpiration;
    String json;
    serializeJson(doc, json);  // Serialize to JSON
    return json;
  }

  void fromJson(const String& json) {
    DynamicJsonDocument doc(200);
    deserializeJson(doc, json);
    Username = doc["Username"].as<String>();
    Password = doc["Password"].as<String>();
    SessionId = doc["SessionId"].as<String>();
    SessionExpiration = doc["SessionExpiration"].as<unsigned long>();
  }
  bool isPasswordIsCorrect(String pss) {
    return Password == pss;
  }

  void setPassword(String pass) {
    Password = pass;
  }

  String getPassword() {
    return Password;
  }

  void setUserName(String UserName) {
    Username = UserName;
  }
  void setSessionId(String SID) {
    SessionId = SID;
  }

  void setSessionExpiresion(unsigned long sessionEXP) {
    SessionExpiration = sessionEXP;
  }

  String getSessionId(){
    return SessionId;
  }
  unsigned long getSessionExpiration() {
    return SessionExpiration;
  }
};

class UserManager {
private:
  std::vector<User> users;
public:
  UserManager() {}
  void loadUsersFromFile(SDClass* sd);
  void saveUsersToFile(SDClass* sd);
  bool removeUser(const String& userName);
  User* findUser(String username);
  User* findUserByIp(String username);
  String padString(String str, int fixedLength);
  String encryptPassword(String plainPassword);
  byte* toByte(String str);
  String stringToHex(String input);
  String hexToString(String hexInput);
  void createSession(String username);
  void handleProtectedResource(String username);
  bool isSessionValid(String username);
  String generateSecureSessionId();
  void addUser(User* u);
};
void UserManager::saveUsersToFile(SDClass* sd) {
  File file = sd->open(DIR_PATH[FILE_TYPE::USERS_JSON] + CONFIG_FILE_NAMES[CONF_SECTION::USERS_JSON_FILE], FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  // Save each user as a JSON string in the file
  for (const User& user : users) {
    file.println(user.toJson());
  }
  file.close();
  Serial.println("Users saved to file");
}

void UserManager::loadUsersFromFile(SDClass* sd) {
  String fileName = DIR_PATH[FILE_TYPE::USERS_JSON] + CONFIG_FILE_NAMES[CONF_SECTION::USERS_JSON_FILE];
  //Serial.println(fileName);
  File file = sd->open(fileName, FILE_READ);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  // Clear the existing users vector
  users.clear();

  // Read each line (user) and deserialize the JSON back into user objects
  while (file.available()) {
    //
    String line = file.readStringUntil('\n');
    User user;
    user.fromJson(line);
    //Serial.println(user.getUsername());
    users.push_back(user);
  }
  file.close();
  //Serial.println("Users loaded from file");
}

String UserManager::encryptPassword(String Password) {
  experimental::crypto::SHA256 hash;
  return hash.hash(PREFIX_SALT + Password + POSTFIX_SALT);
}

void UserManager::addUser(User* u) {
  users.push_back(*u);
}

bool UserManager::removeUser(const String& userName) {
  for (auto it = users.begin(); it != users.end(); ++it) {
    if (it->getUsername() == userName) {
      users.erase(it);
      return true;
    }
  }
  return false;
}

User* UserManager::findUser(String username) {
  for (auto& user : users) {
    if (user.getUsername() == username) {
      return &user;  // بازگشت اشاره‌گر به کاربر
    }
  }
  return nullptr;  // کاربر پیدا نشد
}


User* UserManager::findUserByIp(String ipAddr) {
  for (auto& user : users) {
    if (user.IPAddr == ipAddr && user.isLogin) {
      return &user;  // بازگشت اشاره‌گر به کاربر
    }
  }
  return nullptr;  // کاربر پیدا نشد
}


String UserManager::padString(String str, int fixedLength) {

  if (str.length() >= fixedLength) {
    return str.substring(0, fixedLength);  // Truncate if too long
  }

  while (str.length() < fixedLength) {
    str += ' ';  // Append a space character
  }

  return str;
}


byte* UserManager::toByte(String str) {
  int length = str.length();
  byte byteArray[length];

  // Fill the byte array with the string's data
  for (int i = 0; i < length; i++) {
    byteArray[i] = (byte)str[i];  // Convert char to byte
  }

  return byteArray;
}

String UserManager::stringToHex(String input) {
  String hexString = "";
  for (size_t i = 0; i < input.length(); i++) {
    // Convert each character to its hexadecimal value
    char hex[3];
    snprintf(hex, sizeof(hex), "%02X", (unsigned char)input.charAt(i));  // %02X for 2-character hex with zero-padding
    hexString += hex;                                                    // Append the hex value to the result string
  }
  return hexString;
}

String UserManager::hexToString(String hexInput) {
  String output = "";

  if (hexInput.length() % 2 != 0) {
    return output;  // Return an empty string for invalid input
  }

  for (size_t i = 0; i < hexInput.length(); i += 2) {
    // Get two characters at a time

    String byteString = hexInput.substring(i, i + 2);
    char character = (char)strtol(byteString.c_str(), NULL, 16);
    output += character;
  }

  return output;
}

void UserManager::createSession(String username) {
  User* user = findUser(username);
  if (user) {
    user->setSessionId(generateSecureSessionId());
    user->setSessionExpiresion(millis() + 1800000);  // 30 دقیقه
  }
}

void UserManager::handleProtectedResource(String username) {
  if (isSessionValid(username)) {
    // دسترسی به منبع محافظت‌شده
  } else {
    // درخواست ورود دوباره کاربر
  }
}

bool UserManager::isSessionValid(String username) {
  User* user = findUser(username);
  if (user && millis() < user->getSessionExpiration()) {
    return true;  // سشن معتبر است
  }
  return false;  // سشن نامعتبر است
}

// تابع برای تولید SessionId ایمن
String UserManager::generateSecureSessionId() {
  const char* key = "HaLATA";
  const char* payload = "INTHENAMEOFGOD";
  byte hmacResult[32];

  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;

  const size_t payloadLength = strlen(payload);
  const size_t keyLength = strlen(key);

  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 1);
  mbedtls_md_hmac_starts(&ctx, (const unsigned char*)key, keyLength);
  mbedtls_md_hmac_update(&ctx, (const unsigned char*)payload, payloadLength);
  mbedtls_md_hmac_finish(&ctx, hmacResult);
  mbedtls_md_free(&ctx);

  String sessionId;
  for (int i = 0; i < sizeof(hmacResult); i++) {

    char str[3];

    sprintf(str, "%02x", (int)hmacResult[i]);
    sessionId += str;
  }

  return sessionId;
}
