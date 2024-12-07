#include "LocalUser.h"

LocalUser::LocalUser() {
    checkRegistration();
}

void LocalUser::checkRegistration() {
    std::ifstream infile("my.info");
    if (!infile.is_open()) {
        registered = false;
        return;
    }

    std::string name, clientID, privateKey;
    if (!std::getline(infile, name) || !std::getline(infile, clientID) || !std::getline(infile, privateKey)) {
        std::cerr << "Error: Incomplete data in my.info file, either fix or delete it.\n";
        exit(0);
    }

    infile.close();
    registered = true;
    this->name = name;
    this->clientID = clientID;
    std::cout << this->clientID << std::endl;
}

bool LocalUser::saveToFile() {
    std::ofstream outfile("my.info", std::ios::binary);
    if (!outfile.is_open()) return false;

    outfile << name << "\n";
    outfile << Utils::bytesToHex(clientID) << "\n";

    outfile.close();
    return true;
}

std::string LocalUser::getName() const {
    return name;
}

void LocalUser::setName(const std::string& name) {
    this->name = name;
}

std::string LocalUser::getClientID() const {
    return clientID;
}

void LocalUser::setClientID(const std::string& id) {
    clientID = id;
}

bool LocalUser::isRegistered() const {
    return registered;
}

void LocalUser::setRegistered(bool status) {
    registered = status;
}