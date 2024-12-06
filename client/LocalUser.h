#pragma once

#include "UserInfo.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include "Base64Wrapper.h"
#include "Utils.h"

class LocalUser {
private:
    std::string name;
    std::string clientID;
    std::string publicKey;
    std::string privateKey;
    bool registered;

public:
    LocalUser();

    void checkRegistration();

    bool saveToFile();

    std::string getPrivateKey() const;
    void setPrivateKey(const std::string& privateKey);


    bool isRegistered() const;
    void setRegistered(bool status);

    std::string getName() const;
    void setName(const std::string& name);

    std::string getClientID() const;
    void setClientID(const std::string& ClientID);

    std::string getPublicKey() const;
    void setPublicKey(const std::string& publicKey);

};
