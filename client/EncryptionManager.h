#pragma once

#include "RSAWrapper.h"
#include "AESWrapper.h"
#include <string>
#include <unordered_map>
#include <iostream>
#include <iomanip>

class EncryptionManager {
public:
    EncryptionManager();

    std::string getPublicKey() const;
    std::string getPrivateKey() const;

    std::string encryptWithPublicKey(const std::string& publicKey, const std::string& message);
    std::string decryptWithPrivateKey(const std::string& cipher);

    void generateSymmetricKey(const std::string& clientID);
    std::string getSymmetricKey(const std::string& clientID) const;
    void storeSymmetricKey(const std::string& clientID, const std::string& key);

    std::string encryptWithSymmetricKey(const std::string& clientID, const std::string& message);
    std::string decryptWithSymmetricKey(const std::string& clientID, const std::string& cipher);

private:
    RSAPrivateWrapper rsaPrivate; 
    std::unordered_map<std::string, AESWrapper> symmetricKeys;
};
