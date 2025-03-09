#include "UserInfoList.h"


UserInfoList::UserInfoList() {
}

UserInfo& UserInfoList::getUserByID(const std::string& clientID) {
	for (UserInfo& user : userInfoList) {
		if (user.getClientID() == clientID) {
			return user;
		}
	}
	throw ClientException(ClientErrorCode::USER_NOT_FOUND);
}

UserInfo& UserInfoList::getUserByName(const std::string& name) {
	for (UserInfo& user : userInfoList) {
		if (!name.compare(user.getName())) {
			return user;
		}
	}
	throw ClientException(ClientErrorCode::USER_NOT_FOUND);
}

void UserInfoList::addUser(const std::string& clientID, const std::string& name) {
	for (const auto& user : userInfoList) {
		if (user.getClientID() == clientID) {
			return;
		}
	}

	UserInfo newUser(name, clientID);
	userInfoList.push_back(newUser);
}

void UserInfoList::printUsers(UserInterface& userInterface) {
	userInterface.printText("Registered clients:");
	for (const UserInfo& user : userInfoList) {
		userInterface.printText(user.toString());
	}
}
