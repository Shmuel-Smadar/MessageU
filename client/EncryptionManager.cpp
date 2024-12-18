#include "EncryptionManager.h"

EncryptionManager::EncryptionManager()
    : rsaPrivate(),
    rsaPublic(rsaPrivate.getPublicKey()) {}

EncryptionManager::EncryptionManager(const std::string& privateKeyStr)
    : rsaPrivate(privateKeyStr),
    rsaPublic(rsaPrivate.getPublicKey()) {}

std::string EncryptionManager::getPublicKey() const
{
    return rsaPublic.getPublicKey();
}

std::string EncryptionManager::getPrivateKey() const
{
    return rsaPrivate.getPrivateKey();
}

void EncryptionManager::storePublicKey(const std::string& clientID, std::string& key) {
    publicKeys[clientID] = std::make_unique<RSAPublicWrapper>(key);
}

std::string EncryptionManager::encryptWithPublicKey(const std::string& clientID, const std::string& message)
{
    auto it = publicKeys.find(clientID);
    if (it != publicKeys.end()) {
        return it->second->encrypt(message);
    }
    return "";
}

std::string EncryptionManager::decryptWithPrivateKey(const std::string& ciphertext)
{
    return rsaPrivate.decrypt(ciphertext);
}

void EncryptionManager::generateSymmetricKey(const std::string& clientID) {
    unsigned char key[AESWrapper::DEFAULT_KEYLENGTH];
    AESWrapper::GenerateKey(key, AESWrapper::DEFAULT_KEYLENGTH);
    symmetricKeys[clientID] = std::make_unique<AESWrapper>(key, AESWrapper::DEFAULT_KEYLENGTH);
}

std::string EncryptionManager::getSymmetricKey(const std::string& clientID) const {
    auto it = symmetricKeys.find(clientID);
    if (it != symmetricKeys.end()) {
        const unsigned char* key = it->second->getKey();
        return std::string(reinterpret_cast<const char*>(key), AESWrapper::DEFAULT_KEYLENGTH);
    }
    return "";
}

void EncryptionManager::storeSymmetricKey(const std::string& clientID, const std::string& key) {
    symmetricKeys[clientID] = std::make_unique<AESWrapper>(reinterpret_cast<const unsigned char*>(key.c_str()), AESWrapper::DEFAULT_KEYLENGTH);
}

std::string EncryptionManager::encryptWithSymmetricKey(const std::string& clientID, const std::string& message) {
    auto it = symmetricKeys.find(clientID);
    if (it != symmetricKeys.end()) {
        return it->second->encrypt(message.c_str(), message.size());
    }
    return "";
}

std::string EncryptionManager::decryptWithSymmetricKey(const std::string& clientID, const std::string& ciphertext) {
    auto it = symmetricKeys.find(clientID);
    if (it != symmetricKeys.end()) {
        return it->second->decrypt(ciphertext.c_str(), ciphertext.size());
    }
    return "";
}