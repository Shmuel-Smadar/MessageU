#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include "Message.h"
#include "CurrentUser.h"
#include "Utils.h"
#include "UserInfoList.h"
#include "EncryptionManager.h"
#include "ProtocolConstants.h"

class RequestBuilder {
private:
	uint8_t clientVersion = 1;

	std::vector<uint8_t> buildRequestHeaders(const CurrentUser& currentUser);

public:
	std::vector<uint8_t> buildRegistrationRequest(const CurrentUser& currentUser, const std::string& publicKey);
	std::vector<uint8_t> buildClientsListRequest(CurrentUser& currentUser);
	std::vector<uint8_t> buildPublicKeyRequest(CurrentUser& currentUser, UserInfo& userInfo);
	std::vector<uint8_t> buildWaitingMessagesRequest(CurrentUser& currentUser);
	std::vector<uint8_t> buildSymmetricKeyRequest(CurrentUser& currentUser, UserInfo& userInfo, EncryptionManager& encryptionManager);
	std::vector<uint8_t> buildTextMessageRequest(CurrentUser& currentUser, UserInfo& userInfo, std::string& textMessage, EncryptionManager& encryptionManager);
	std::vector<uint8_t> buildSendSymmetricKey(CurrentUser& currentUser, UserInfo& userInfo, EncryptionManager& encryptionManager);
};