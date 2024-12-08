#include "UserManager.h"

UserManager::UserManager()
    : userInfoList(),
    encryptionManager(nullptr) {
    checkRegistration();
}

void UserManager::checkRegistration() {
    std::ifstream infile("my.info");
    if (!infile.is_open()) {
        localUser = LocalUser();
        encryptionManager = std::make_unique<EncryptionManager>(); 
        return;
    }

    std::string name, clientID, privateKey;
    if (!std::getline(infile, name) || !std::getline(infile, clientID) || !std::getline(infile, privateKey)) {
        std::cerr << "Error: Incomplete data in my.info file.\n";
        exit(0);
    }

    infile.close();
    localUser = LocalUser(name, clientID);
    encryptionManager = std::make_unique<EncryptionManager>(privateKey);
}