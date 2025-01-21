#include <iostream>
#include <vector>

class User {
private:
  String Username;
  String Password;
public:
  User(String uNmae, String pWord)
    : Username(uName), Password(pWord) {}
  String getUsername(String un) {
    return Username;
  }

  bool isPasswordIsCorrect(String pss) {
    return Password == pss;
  }
};

class UserManager {
private:
  std::pmr::vector<User> users;
public:
  void addUser(const String& uName, const String& pWord) {
    users.emplace_back(uName, pWord);
  }
}