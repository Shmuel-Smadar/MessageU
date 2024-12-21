#include "UserInfo.h"

UserInfo::UserInfo(const std::string& name, const std::string& clientID)
{
	this->name = name;
	this->clientID = clientID;
	this->hasPublicKey = false;
	this->hasSymmetricKey = false;
}

std::string UserInfo::getName() const {
	return name;
}

void UserInfo::setName(const std::string& name) {
	this->name = name;
}

std::string UserInfo::getClientID() const {
	return clientID;
}

void UserInfo::setClientID(const std::string& clientID) {
	this->clientID = clientID;
}

void UserInfo::publicKeyReceived() {
	this->hasPublicKey = true;
}

void UserInfo::symmetricKeyReceived() {
	this->hasSymmetricKey = true;
}
bool UserInfo::isPublicKeyReceived() {
	return this->hasPublicKey;
}
bool UserInfo::isSymmetricKeyReceived() {
	return this->hasSymmetricKey;
}

