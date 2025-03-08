/*
 * File Name: UserInfo.h
 *
 * This file is responsible for storing and managing data and state of other users in the same server.
 */

#pragma once

#include <string>

class UserInfo {

public:

	enum class SymmetricKeyStatus {
		NotRequested,
		RequestedByCurrentUser,
		RequestedByOtherUser,
		RequestedByBoth,
		KeyReceived
	};

	UserInfo(const std::string& name, const std::string& UserID);

	std::string getName() const;
	void setName(const std::string& name);

	std::string getClientID() const;
	void setClientID(const std::string& ClientID);
	std::string toString() const;
	bool isPublicKeyReceived() const;
	bool isSymmetricKeyReceived() const;
	bool isSymmetricKeyRequestedByOtherUser() const;
	bool isSymmetricKeyRequestedByCurrentUser() const;

	void publicKeyReceived();
	void symmetricKeyReceived();
	void otherUserRequestedSymmetricKey();
	void currentUserRequestedSymmetricKey();
private:
	std::string name;
	std::string clientID;
	bool hasPublicKey;
	SymmetricKeyStatus symmetricKeyStatus;



};