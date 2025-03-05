#include "CurrentUser.h"

CurrentUser::CurrentUser() {
    this->registered = false;
}
CurrentUser::CurrentUser(const std::string& name, const std::string& clientID) {
    this->name = name;
    this->clientID = clientID;
    this->registered = true;
}

std::string CurrentUser::getName() const {
    return name;
}

bool CurrentUser::setName(const std::string& name) {
    if (name.length() > ProtocolByteSizes::ClientName + 1) // +1 for the '\0'
        return false;
    this->name = name;
    return true;
}

std::string CurrentUser::getClientID() const {
    return clientID;
}

void CurrentUser::setClientID(const std::string& clientId) {
    this->clientID = clientId;
}

bool CurrentUser::isRegistered() const {
    return registered;
}

void CurrentUser::setRegistered(bool status) {
    registered = status;
}