#include "CurrentUser.h"

CurrentUser::CurrentUser() {
    this->registered = false;
}
CurrentUser::CurrentUser(const std::string& name, const std::string& ClientID) {
    this->name = name;
    this->clientID = clientID;
    this->registered = true;
}

bool CurrentUser::saveToFile() {
    std::ofstream outfile("my.info", std::ios::binary);
    if (!outfile.is_open()) return false;

    outfile << name << "\n";
    outfile << Utils::bytesToHex(clientID) << "\n";

    outfile.close();
    return true;
}

std::string CurrentUser::getName() const {
    return name;
}

void CurrentUser::setName(const std::string& name) {
    this->name = name;
}

std::string CurrentUser::getClientID() const {
    return clientID;
}

void CurrentUser::setClientID(const std::string& id) {
    clientID = id;
}

bool CurrentUser::isRegistered() const {
    return registered;
}

void CurrentUser::setRegistered(bool status) {
    registered = status;
}