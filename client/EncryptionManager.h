#pragma once

#include "RSAWrapper.h"
#include "AESWrapper.h"
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <cstring>

class EncryptionManager
{
private:
    RSAPrivateWrapper rsaPrivate;
    RSAPublicWrapper rsaPublic;
    std::unordered_map<std::string, AESWrapper> symmetricKeys;
    std::unordered_map<std::string, RSAPublicWrapper> publicKeys;

public:
    
    EncryptionManager();
    EncryptionManager(const std::string& privateKeyStr);
    EncryptionManager(const EncryptionManager&) = delete;
    EncryptionManager& operator=(const EncryptionManager&) = delete;
    

    std::string getPublicKey() const;
    std::string getPrivateKey() const;
    
    std::string encryptWithPublicKey(const std::string& clientID, const std::string& message);
    void storePublicKey(std::string& clientID, std::string& key);
    std::string decryptWithPrivateKey(const std::string& cipher);

    void generateSymmetricKey(const std::string& clientID);
    std::string getSymmetricKey(const std::string& clientID) const;
    void storeSymmetricKey(const std::string& clientID, const std::string& key);

    std::string encryptWithSymmetricKey(const std::string& clientID, const std::string& message);
    std::string decryptWithSymmetricKey(const std::string& clientID, const std::string& cipher);

};
