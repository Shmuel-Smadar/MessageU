/*
 * File Name: CurrentUser.h
 *
 * This file is responsible for managing the state of the current user.
 */

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

class CurrentUser {
private:
    std::string name;
    std::string clientID;
    bool registered;

public:
    CurrentUser();
    CurrentUser(const std::string& name, const std::string& clientID);

    bool isRegistered() const;
    void setRegistered(bool status);

    std::string getName() const;
    bool setName(const std::string& name);

    std::string getClientID() const;
    void setClientID(const std::string& clientID);

};
