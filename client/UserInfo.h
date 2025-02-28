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