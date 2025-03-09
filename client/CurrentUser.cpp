#include "CurrentUser.h"

CurrentUser::CurrentUser() {
	this->registered = false;
}
CurrentUser::CurrentUser(const std::string& name, const std::string& clientID) {
	this->name = name;
	this->clientID = clientID;
	this->registered = true;
}

std::string CurrentUser::getName() const {
	return name;
}

void CurrentUser::setName(const std::string& name) {
	if (name.length() > ProtocolByteSizes::ClientName + 1) // +1 for the '\0'
		throw ClientException(ClientErrorCode::NAME_TOO_LONG);
	this->name = name;
}

std::string CurrentUser::getClientID() const {
	return clientID;
}

void CurrentUser::setClientID(const std::string& clientId) {
	this->clientID = clientId;
}

bool CurrentUser::isRegistered() const {
	return registered;
}

void CurrentUser::setRegistered(bool status) {
	registered = status;
}