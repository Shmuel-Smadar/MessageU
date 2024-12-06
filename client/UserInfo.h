#pragma once

#include <string>

class UserInfo {
private:
    std::string name;
    std::string clientID;
    std::string publicKey;
    std::string symmetricKey;

public:
    UserInfo(const std::string& name, const std::string& UserID);

    std::string getName() const;
    void setName(const std::string& name);

    std::string getClientID() const;
    void setClientID(const std::string& ClientID);

    std::string getPublicKey() const;
    void setPublicKey(const std::string& publicKey);

    std::string getSymmetricKey() const;
    void setSymmetricKey(const std::string& symmetricKey);


};