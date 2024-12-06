#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include "UserInfo.h"


class UserInfoList {
private:
    std::vector<UserInfo> userInfoList;

public:
    UserInfoList();

    UserInfo* getUserByID(const std::string& UserID);
    UserInfo* getUserByName(const std::string& name);
    void addUser(const std::string& UserID, const std::string& name);

    void addUserPublicKey(const std::string& UserID, const std::string& publicKey);
    std::string getUserPublicKey(const std::string& UserID) const;
    void printUsers();
};
