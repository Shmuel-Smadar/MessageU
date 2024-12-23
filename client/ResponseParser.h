#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include "CurrentUser.h"
#include "Utils.h"
#include "UserInfo.h"
#include "UserInfoList.h"
#include "EncryptionManager.h"
#include "ProtocolConstants.h"

class ResponseParser {
private:
	uint8_t clientVersion = 1;

	std::unique_ptr<ResponseHeader> parseResponseHeaders(const std::vector<uint8_t>& data);

public:
	bool parseRegistrationResponse(const std::vector<uint8_t>& data, CurrentUser& currentUser);
	bool parseClientsListResponse(const std::vector<uint8_t>& data, UserInfoList& userInfoList);
	bool parsePublicKeyResponse(const std::vector<uint8_t>& data, UserInfo& userInfo, EncryptionManager& encryptionManager);
	std::vector<Message> parseAwaitingMessagesResponse(const std::vector<uint8_t>& data, UserInfoList& userInfoList, EncryptionManager& encryptionManager);
	bool parseSymmetricKeyRequestResponse(const std::vector<uint8_t>& data, const UserInfo& userInfo);
	void parseMessage(const Message message, EncryptionManager& encryptionManager);
};