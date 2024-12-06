#include "UserInfo.h"

#include <iostream>

UserInfo::UserInfo(const std::string& name, const std::string& clientID)
{
    this->name = name;
    this->clientID = clientID;
    this->publicKey = "";
    this->symmetricKey = "";
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

std::string UserInfo::getPublicKey() const {
    return publicKey;
}

void UserInfo::setPublicKey(const std::string& publicKey) {
    this->publicKey = publicKey;
}


std::string UserInfo::getSymmetricKey() const {
    return symmetricKey;
}

void UserInfo::setSymmetricKey(const std::string& symmetricKey) {
    this->symmetricKey = symmetricKey;
}



