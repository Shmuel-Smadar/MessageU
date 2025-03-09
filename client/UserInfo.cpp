#include "UserInfo.h"

UserInfo::UserInfo(const std::string& name, const std::string& clientID)
{
	this->name = name;
	this->clientID = clientID;
	this->hasPublicKey = false;
	symmetricKeyStatus = SymmetricKeyStatus::NotRequested;
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
	this->symmetricKeyStatus = SymmetricKeyStatus::KeyReceived;
}
void UserInfo::otherUserRequestedSymmetricKey() {
	if (this->symmetricKeyStatus == SymmetricKeyStatus::RequestedByCurrentUser) {
		this->symmetricKeyStatus = SymmetricKeyStatus::RequestedByBoth;
	}
	else if (this->symmetricKeyStatus == SymmetricKeyStatus::NotRequested) {
		this->symmetricKeyStatus = SymmetricKeyStatus::RequestedByOtherUser;
	}
}

void UserInfo::currentUserRequestedSymmetricKey() {
	if (this->symmetricKeyStatus == SymmetricKeyStatus::RequestedByOtherUser) {
		this->symmetricKeyStatus = SymmetricKeyStatus::RequestedByBoth;
	}
	else if (this->symmetricKeyStatus == SymmetricKeyStatus::NotRequested) {
		this->symmetricKeyStatus = SymmetricKeyStatus::RequestedByCurrentUser;
	}

}

bool UserInfo::isPublicKeyReceived() const {
	return this->hasPublicKey;
}
bool UserInfo::isSymmetricKeyReceived() const {
	return this->symmetricKeyStatus == SymmetricKeyStatus::KeyReceived;
}
bool UserInfo::isSymmetricKeyRequestedByOtherUser() const {
	return (this->symmetricKeyStatus == SymmetricKeyStatus::RequestedByOtherUser);
}
bool UserInfo::isSymmetricKeyRequestedByCurrentUser() const {
	return (this->symmetricKeyStatus == SymmetricKeyStatus::RequestedByCurrentUser);
}

std::string UserInfo::toString() const {
	std::string userString = "Name: " + this->getName() + " ID: " + this->getClientID();
	return userString;
}