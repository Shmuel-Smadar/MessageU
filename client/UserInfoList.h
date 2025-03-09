/*
 * File Name: UserInfoList.h
 *
 * This file is responsible for managing a list of UserInfo objects
 * for easing the management of all the users.
 */


#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include "UserInfo.h"
#include "ClientException.h"
#include "ErrorCodes.h"
#include "UserInterface.h"

class UserInfoList {
private:
	std::vector<UserInfo> userInfoList;

public:
	UserInfoList();

	UserInfo& getUserByID(const std::string& UserID);
	UserInfo& getUserByName(const std::string& name);
	void addUser(const std::string& UserID, const std::string& name);
	void printUsers(UserInterface& userInterface);
};
