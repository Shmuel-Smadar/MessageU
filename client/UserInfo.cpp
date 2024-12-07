#include "UserInfo.h"

#include <iostream>

UserInfo::UserInfo(const std::string& name, const std::string& clientID)
{
    this->name = name;
    this->clientID = clientID;
}


std::string UserInfo::getName() const {
    return name;
}

void UserInfo::setName(const std::string& name) {
    this->name = name;
}

std::string UserInfo::getClientID() const {
    return clientID;
}

void UserInfo::setClientID(const std::string& clientID) {
    this->clientID = clientID;
}



