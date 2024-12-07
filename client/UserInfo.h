#pragma once

#include <string>

class UserInfo {
private:
    std::string name;
    std::string clientID;

public:
    UserInfo(const std::string& name, const std::string& UserID);

    std::string getName() const;
    void setName(const std::string& name);

    std::string getClientID() const;
    void setClientID(const std::string& ClientID);


};