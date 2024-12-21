#pragma once

#include <string>

class UserInfo {
private:
    std::string name;
    std::string clientID;
    bool hasPublicKey;
    bool hasSymmetricKey;

public:
    UserInfo(const std::string& name, const std::string& UserID);

    std::string getName() const;
    void setName(const std::string& name);

    std::string getClientID() const;
    void setClientID(const std::string& ClientID);
    void publicKeyReceived();
    void symmetricKeyReceived();
    bool isPublicKeyReceived();
    bool isSymmetricKeyReceived();


};