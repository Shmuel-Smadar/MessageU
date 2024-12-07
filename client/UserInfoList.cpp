#include "UserInfoList.h"


UserInfoList::UserInfoList() {
}

UserInfo* UserInfoList::getUserByID(const std::string& clientID) {
    for (UserInfo& user : userInfoList) {
        if (user.getClientID() == clientID) {
            return &user;
        }
    }
    return nullptr;
}

UserInfo* UserInfoList::getUserByName(const std::string& name) {
    for (UserInfo& user : userInfoList) {
        std::string n = user.getName();
        if (name == n) {
            return &user;
        }
    }
    return nullptr;
}

void UserInfoList::addUser(const std::string& clientID, const std::string& name) {
    for (const auto& User : userInfoList) {
        if (User.getClientID() == clientID) {
            return;
        }
    }

    UserInfo newUser(name, clientID);
    printUsers();
    userInfoList.push_back(newUser);
}

void UserInfoList::printUsers() {
    for (const auto& user : userInfoList) {
        std::cout << user.getName() << std::endl;
        std::cout << user.getClientID() << std::endl;
    }

}
